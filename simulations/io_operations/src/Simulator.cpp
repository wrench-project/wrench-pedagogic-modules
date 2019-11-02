#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <algorithm>

#include <simgrid/s4u.hpp>
#include <wrench.h>
#include <nlohmann/json.hpp>
#include <pugixml.hpp>

#include "ActivityWMS.h"
#include "ActivityScheduler.h"

/**
 * @brief Generates an independent-task Workflow
 *
 * @param workflow
 * @param task_read: task read size in MB
 * @param task_write: task write size in MB
 * @param task_num: number of tasks
 * @param task_gflop: Task GFlop rating
 * @param io_overlap: Whether IO overlaps w/ computation.
 *
 * @throws std::invalid_argument
 */
void generateWorkflow(wrench::Workflow *workflow, int task_read, int task_write, int task_num, int task_gflop, bool io_overlap) {

    if (workflow == nullptr) {
        throw std::invalid_argument("generateWorkflow(): invalid workflow");
    }

    if (task_read < 0) {
        throw std::invalid_argument("generateWorkflow(): task read must be 0 or above");
    }

    if (task_write < 0) {
        throw std::invalid_argument("generateWorkflow(): task write must be 0 or above");
    }

    if (task_num <= 0) {
        throw std::invalid_argument("generateWorkflow(): task num must be greater than 0");
    }

    if (task_gflop < 1) {
        throw std::invalid_argument("generateWorkflow(): task GFlop must be at least 1");
    }

    if (io_overlap != true && io_overlap != false){
        throw std::invalid_argument("generateWorkflow(): io overlap must be a boolean");
    }

    // WorkflowTask specifications
    const double               GFLOP = 1000.0 * 1000.0 * 1000.0;
    const unsigned long    MIN_CORES = 1;
    const unsigned long    MAX_CORES = 1;
    const double PARALLEL_EFFICIENCY = 1.0; //single core so doesn't really matter
    const double MEMORY_REQUIREMENT  = 0.0; //may change this to variable input later?
    const double                  GB = 1000.0 * 1000.0 * 1000.0;
    const double                  MB = 1000.0 * 1000.0;



    // create the tasks, need to have computation tasks as well as R/W tasks. If overlaps, IO can run in separate tasks.
    if (io_overlap == true){
        for (int i = 0; i < task_num; ++i){
            std::string io_read_task_id("io read task #" + std::to_string(i));
            workflow->addTask(io_read_task_id, 0.0, 0, 0, PARALLEL_EFFICIENCY, MEMORY_REQUIREMENT);
            io_read_task_id->addInputFile(workflow->addFile(io_read_task_id+"::0.in", task_read * MB));
            if (i>0) {
                workflow->addControlDependency(workflow->getTaskByID(io_read_task_id), workflow->getTaskByID("io write task #" + std::to_string(i-1)))
            }

            std::string compute_task_id("compute task #" + std::to_string(i));
            workflow->addTask(compute_task_id, task_gflop * GFLOP, MIN_CORES, MAX_CORES, PARALLEL_EFFICIENCY, MEMORY_REQUIREMENT);
            workflow->addControlDependency(workflow->getTaskByID(compute_task_id), workflow->getTaskByID(io_read_task_id))

            std::string io_write_task_id("io write task #" + std::to_string(i));
            workflow->addTask(io_write_task_id, 0.0, 0, 0, PARALLEL_EFFICIENCY, MEMORY_REQUIREMENT);
            io_write_task_id->addOutputFile(workflow->addFile(io_write_task_id+"::0.out", task_write * MB));
            workflow->addControlDependency(workflow->getTaskByID(io_write_task_id), workflow->getTaskByID(compute_task_id))
        }
    } else {
        for (int i = 0; i < task_num; ++i) {
            std::string task_id("task #" + std::to_string(i));
            workflow->addTask(task_id, task_gflop * GFLOP, MIN_CORES, MAX_CORES, PARALLEL_EFFICIENCY, MEMORY_REQUIREMENT);
            task_id->addInputFile(workflow->addFile(task_id+"::0.in", task_read * MB));
            task_id->addOutputFile(workflow->addFile(task_id+"::0.out", task_write * MB));
        }

    }

}

/**
 * @brief Generates a platform with a single multi-core host
 * @param platform_file_path: path to write the platform file to
 *
 * @throws std::invalid_argumemnt
 */
void generatePlatform(std::string platform_file_path) {

    if (platform_file_path.empty()) {
        throw std::invalid_argument("generatePlatform() platform_file_path cannot be empty");
    }

    // Create a the platform file
    //TODO add disk
    std::string xml = "<?xml version='1.0'?>\n"
                      "<!DOCTYPE platform SYSTEM \"http://simgrid.gforge.inria.fr/simgrid/simgrid.dtd\">\n"
                      "<platform version=\"4.1\">\n"
                      "   <zone id=\"AS0\" routing=\"Full\">\n"
                      "       <host id=\"io_host\" speed=\"100Gf\" core=\"1\">\n"
                      "           <prop id=\"ram\" value=\"32GB\"/>\n"
                      "           <disk id=\"large_disk\" read_bw=\"150MBps\" write_bw=\"100MBps\">\n"
                      "                            <prop id=\"size\" value=\"5000GiB\"/>\n"
                      "                            <prop id=\"mount\" value=\"/\"/>\n"
                      "           </disk>\n"
                      "       </host>\n"
                      "       <link id=\"link\" bandwidth=\"100000TBps\" latency=\"0us\"/>\n"
                      "       <route src=\"io_host\" dst=\"io_host\">\n"
                      "           <link_ctn id=\"link\"/>\n"
                      "       </route>\n"
                      "   </zone>\n"
                      "</platform>\n";

    FILE *platform_file = fopen(platform_file_path.c_str(), "w");
    fprintf(platform_file, "%s", xml.c_str());
    fclose(platform_file);
}

/**
 *
 * @param argc
 * @param argvx
 * @return
 */
int main(int argc, char** argv) {

    wrench::Simulation simulation;
    simulation.init(&argc, argv);

    const int MAX_CORES         = 1000;
    const int NUM_CORES         = 1;
    int TASK_READ;
    int TASK_WRITE;
    int TASK_NUM;
    int TASK_GFLOP;
    bool IO_OVERLAP;

    try {

        if (argc != 6) {
            throw std::invalid_argument("bad args");
        }

        TASK_READ = std::stoi(std::string(argv[1]));

        if (TASK_READ < 0) {
            std::cerr << "Invalid task read. Enter a value greater than 0" << std::endl;
            throw std::invalid_argument("invalid task read amount");
        }

        TASK_WRITE = std::stoi(std::string(argv[2]));

        if (TASK_WRITE < 0) {
            std::cerr << "Invalid task write. Enter a value greater than 0" << std::endl;
            throw std::invalid_argument("invalid task write amount");
        }

        TASK_NUM = std::stoi(std::string(argv[3]));

        if (TASK_NUM<1) {
            std::cerr << "Invalid task num. Enter a value greater than 0" << std::endl;
            throw std::invalid_argument("invalid task num amount");
        }

        TASK_GFLOP = std::stoi(std::string(argv[4]));

        if (TASK_GFLOP < 1) {
            std::cerr << "Invalid task gflop. Enter a value greater than 1" << std::endl;
            throw std::invalid_argument("invalid task gflop");
        }

        IO_OVERLAP = std::stoi(std::string(argv[5]));

        if (IO_OVERLAP != true && IO_OVERLAP != false) {
            std::cerr << "Invalid io overlap. Enter a boolean value" << std::endl;
            throw std::invalid_argument("invalid io overlap");
        }


    } catch(std::invalid_argument &e) {
        std::cerr << "Usage: " << argv[0] << " <task_read> <task_write> <task_gflop> <io_overlap>" << std::endl;
        std::cerr << "   task_read: amount read per task in MB [0,9999]" << std::endl;
        std::cerr << "   task_write: amount written per task in MB [0,9999]" << std::endl;
        std::cerr << "   task_num: number of tasks to compute (>0)" << std::endl;
        std::cerr << "   task_gflop: task GFlop (> 0)" << std::endl;
        std::cerr << "   io_overlap: whether read/write can be done concurrently with computation" << std::endl;
        std::cerr << "" << std::endl;
        std::cerr << "   (Core speed is always 100GFlop/sec, Host RAM capacity is always 32 GB)" << std::endl;
        return 1;
    }

    // create workflow
    wrench::Workflow workflow;
    generateWorkflow(&workflow, TASK_READ, TASK_WRITE, TASK_NUM, TASK_GFLOP, IO_OVERLAP);

    // read and instantiate the platform with the desired HPC specifications
    std::string platform_file_path = "/tmp/platform.xml";
    generatePlatform(platform_file_path);

    simulation.instantiatePlatform(platform_file_path);


    const std::string WMS_HOST("io_host");
    const std::string COMPUTE_HOST("io_host");
    const std::string STORAGE_HOST("io_host");

    auto io_storage_service = simulation.add(new wrench::SimpleStorageService(STORAGE_HOST, {"/"}));

    auto compute_service = simulation.add(
            new wrench::BareMetalComputeService(
                    COMPUTE_HOST,
                    {{COMPUTE_HOST, std::make_tuple(NUM_CORES, wrench::ComputeService::ALL_RAM)}},
                    0,
                    {
                            {wrench::BareMetalComputeServiceProperty::THREAD_STARTUP_OVERHEAD, "0"},
                    },
                    {}
            )
    );

    simulation.add(new wrench::FileRegistryService(WMS_HOST));

    // wms
    auto wms = simulation.add(new wrench::ActivityWMS(std::unique_ptr<wrench::ActivityScheduler>(
            new wrench::ActivityScheduler(io_storage_service)),
            {compute_service},
            {io_storage_service},
            {},
            WMS_HOST
    ));

    wms->addWorkflow(&workflow);



    // stage the input files
    std::map<std::string, wrench::WorkflowFile *> input_files = workflow.getInputFiles();
    simulation.stageFiles(input_files, io_storage_service);


    simulation.launch();

    simulation.getOutput().dumpWorkflowExecutionJSON(&workflow, "workflow_data.json", true);
    //simulation.getOutput().dumpWorkflowGraphJSON(&workflow, "workflow_graph.json");

    return 0;
}

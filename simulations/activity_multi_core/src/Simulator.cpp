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
 * @param num_tasks: number of tasks
 * @param task_gflop: Task GFlop rating
 *
 * @throws std::invalid_argument
 */
void generateWorkflow(wrench::Workflow *workflow, int num_tasks, int task_gflop, bool requires_memory) {

    if (workflow == nullptr) {
        throw std::invalid_argument("generateWorkflow(): invalid workflow");
    }

    if (num_tasks < 1) {
        throw std::invalid_argument("generateWorkflow(): number of tasks must be at least 1");
    }

    if (task_gflop < 1) {
        throw std::invalid_argument("generateWorkflow(): task GFlop must be at least 1");
    }

    // WorkflowTask specifications
    const double               GFLOP = 1000.0 * 1000.0 * 1000.0;
    const unsigned long    MIN_CORES = 1;
    const unsigned long    MAX_CORES = 1;
    const double PARALLEL_EFFICIENCY = 1.0;
    const double  MEMORY_REQUIREMENT = requires_memory ? 12.0 * 1000.0 * 1000.0 * 1000.0 : 0.0;

    // create the tasks
    for (int i = 0; i < num_tasks; ++i) {
        std::string task_id("task #" + std::to_string(i));
        workflow->addTask(task_id, task_gflop * GFLOP, MIN_CORES, MAX_CORES, PARALLEL_EFFICIENCY, MEMORY_REQUIREMENT);
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
    std::string xml = "<?xml version='1.0'?>\n"
                      "<!DOCTYPE platform SYSTEM \"http://simgrid.gforge.inria.fr/simgrid/simgrid.dtd\">\n"
                      "<platform version=\"4.1\">\n"
                      "   <zone id=\"AS0\" routing=\"Full\">\n"
                      "       <host id=\"the_host\" speed=\"100Gf\" core=\"1000\"/>\n"
                      "       <link id=\"link\" bandwidth=\"100000TBps\" latency=\"0us\"/>\n"
                      "       <route src=\"the_host\" dst=\"the_host\">"
                      "           <link_ctn id=\"link\"/>"
                      "       </route>"
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
    int NUM_CORES;
    int NUM_TASKS;
    int TASK_GFLOP;
    bool REQUIRES_MEMORY;

    try {

        if (argc != 5) {
            throw std::invalid_argument("bad args");
        }

        NUM_CORES = std::stoi(std::string(argv[1]));

        if (NUM_CORES < 1 || NUM_CORES > MAX_CORES) {
            std::cerr << "Invalid number cores. Enter a value in the range [1, " + std::to_string(MAX_CORES) + "]" << std::endl;
            throw std::invalid_argument("invalid number of cores");
        }

        NUM_TASKS = std::stoi(std::string(argv[2]));

        if (NUM_TASKS < 1) {
            std::cerr << "Invalid number tasks. Enter a value greater than 1" << std::endl;
            throw std::invalid_argument("invalid number of tasks");
        }

        TASK_GFLOP = std::stoi(std::string(argv[3]));

        if (TASK_GFLOP < 1) {
            std::cerr << "Invalid task gflop. Enter a value greater than 1" << std::endl;
            throw std::invalid_argument("invalid task gflop");
        }

        REQUIRES_MEMORY = std::stoi(std::string(argv[4]));


    } catch(std::invalid_argument &e) {
        std::cerr << "Usage: " << argv[0] << " <num cores> <num tasks> <task gflop>" << std::endl;
        std::cerr << "   num_cores: number of cores [1, " + std::to_string(MAX_CORES) + "]" << std::endl;
        std::cerr << "   num_tasks: number of tasks (> 0)" << std::endl;
        std::cerr << "   task_gflop: task gflop (> 0)" << std::endl;
        std::cerr << "   requires_memory: boolean" << std::endl;
        return 1;
    }

    // create workflow
    wrench::Workflow workflow;
    generateWorkflow(&workflow, NUM_TASKS, TASK_GFLOP, REQUIRES_MEMORY);

    // read and instantiate the platform with the desired HPC specifications
    std::string platform_file_path = "/tmp/platform.xml";
    generatePlatform(platform_file_path);
    simulation.instantiatePlatform(platform_file_path);


    const std::string THE_HOST("the_host");
    const double STORAGE_CAPACITY = 50.0 * 1000.0 * 1000.0 * 1000.0;


    auto compute_service = simulation.add(
            new wrench::BareMetalComputeService(
                    THE_HOST,
                    {{THE_HOST, std::make_tuple(NUM_CORES, wrench::ComputeService::ALL_RAM)}},
<<<<<<< HEAD
                    STORAGE_CAPACITY,
=======
                    0, 
                    {},
>>>>>>> b29847aaa8ec4ba07db1988c593a2736596df446
                    {}
            )
    );

    // wms
    auto wms = simulation.add(new wrench::ActivityWMS(std::unique_ptr<wrench::ActivityScheduler>(
            new wrench::ActivityScheduler()), compute_service, THE_HOST
    ));

    wms->addWorkflow(&workflow);

    simulation.launch();

    simulation.getOutput().dumpWorkflowExecutionJSON(&workflow, "workflow_data.json", true);
    //simulation.getOutput().dumpWorkflowGraphJSON(&workflow, "workflow_graph.json");

    return 0;
}

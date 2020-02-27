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
void generateWorkflow(wrench::Workflow *workflow, int host_select) {

    if (workflow == nullptr) {
        throw std::invalid_argument("generateWorkflow(): invalid workflow");
    }

    if (host_select != 1 && host_select != 2) {
        throw std::invalid_argument("generateWorkflow(): valid host must be selected");
    }


    // WorkflowTask specifications
    const double               GFLOP = 1000.0 * 1000.0 * 1000.0;
    const unsigned long    MIN_CORES = 1;
    const unsigned long    MAX_CORES = 1;
    const double PARALLEL_EFFICIENCY = 1.0;
    const double                  GB = 1000.0 * 1000.0 * 1000.0;

    wrench::WorkflowTask *single_task;
    if (host_select == 1) {
        single_task = workflow->addTask("slow_server_task", 10 * GFLOP, MIN_CORES, MAX_CORES, PARALLEL_EFFICIENCY, 8 * GB);
    } else {
        single_task = workflow->addTask("fast_server_task", 10 * GFLOP, MIN_CORES, MAX_CORES, PARALLEL_EFFICIENCY, 8 * GB);
    }
    single_task->addInputFile(workflow->addFile("file_copy", 1*GB));

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
                      "       <!-- effective bandwidth = 10 MBps-->"
                      "       <link id=\"slow_link\" bandwidth=\"10.309MBps\" latency=\"10us\"/>\n"
                      "       <link id=\"fast_link\" bandwidth=\"103.09MBps\" latency=\"10us\"/>\n"
                      "       <host id=\"client\" speed=\"100Gf\" core=\"1000\">\n"
                      "           <prop id=\"ram\" value=\"32GB\"/>\n"
                      "           <disk id=\"large_disk\" read_bw=\"100MBps\" write_bw=\"100MBps\">\n"
                      "                            <prop id=\"size\" value=\"5000GiB\"/>\n"
                      "                            <prop id=\"mount\" value=\"/\"/>\n"
                      "           </disk>\n"
                      "       </host>\n"
                      "       <host id=\"slow_server\" speed=\"10Gf\" core=\"1000\">\n"
                      "           <prop id=\"ram\" value=\"32GB\"/>\n"
                      "       </host>\n"
                      "       <host id=\"fast_server\" speed=\"100Gf\" core=\"1000\">\n"
                      "           <prop id=\"ram\" value=\"32GB\"/>\n"
                      "       </host>\n"
                      "       <link id=\"link\" bandwidth=\"100000TBps\" latency=\"0us\"/>\n"
                      "       <route src=\"client\" dst=\"fast_server\">"
                      "           <link_ctn id=\"slow_link\"/>"
                      "       </route>"
                      "       <route src=\"client\" dst=\"slow_server\">"
                      "           <link_ctn id=\"fast_link\"/>"
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
    int HOST_SELECT;

    try {

        if (argc != 2) {
            throw std::invalid_argument("bad args");
        }

        HOST_SELECT = std::stoi(std::string(argv[1]));

        if (HOST_SELECT !=  1 && HOST_SELECT != 2) {
            std::cerr << "Invalid host selection. Host must be either 1 or 2" << std::endl;
            throw std::invalid_argument("invalid host selection");
        }



    } catch(std::invalid_argument &e) {
        std::cerr << "Usage: " << argv[0] << " <host_select>" << std::endl;
        std::cerr << "   host_select: host selection should be either 1 or 2" << std::endl;
        std::cerr << "" << std::endl;
        return 1;
    }

    // create workflow
    wrench::Workflow workflow;
    generateWorkflow(&workflow, HOST_SELECT);

    // read and instantiate the platform with the desired HPC specifications
    std::string platform_file_path = "/tmp/platform.xml";
    generatePlatform(platform_file_path);
    simulation.instantiatePlatform(platform_file_path);


    const std::string CLIENT("client");
    const std::string SLOW_SERVER("slow_server");
    const std::string FAST_SERVER("fast_server");


    std::set<std::shared_ptr<wrench::StorageService>> storage_services;
    auto client_storage_service = simulation.add(new wrench::SimpleStorageService(CLIENT, {"/"}));
    storage_services.insert(client_storage_service);

    std::shared_ptr<wrench::ComputeService> compute_service;
    if (HOST_SELECT == 1){
        compute_service = simulation.add(
                new wrench::BareMetalComputeService(
                        CLIENT,
                        {{SLOW_SERVER, std::make_tuple(wrench::ComputeService::ALL_CORES, wrench::ComputeService::ALL_RAM)}},
                        "",
                        {
                                {wrench::BareMetalComputeServiceProperty::TASK_STARTUP_OVERHEAD, "0"},
                        },
                        {}
                )
        );
    } else {
        compute_service = simulation.add(
                new wrench::BareMetalComputeService(
                        CLIENT,
                        {{FAST_SERVER, std::make_tuple(wrench::ComputeService::ALL_CORES, wrench::ComputeService::ALL_RAM)}},
                        "",
                        {
                                {wrench::BareMetalComputeServiceProperty::TASK_STARTUP_OVERHEAD, "0"},
                        },
                        {}
                )
        );
    }


    // wms
    auto wms = simulation.add(new wrench::ActivityWMS(std::unique_ptr<wrench::ActivityScheduler>(
            new wrench::ActivityScheduler(client_storage_service)),
                                                      {compute_service},
                    storage_services,
                    CLIENT
    ));





    wms->addWorkflow(&workflow);

    simulation.add(new wrench::FileRegistryService(CLIENT));

    for (auto const &file : workflow.getInputFiles()) {
        simulation.stageFile(file.second, client_storage_service);
    }

    simulation.launch();

    //simulation.getOutput().dumpUnifiedJSON(&workflow, "workflow_data.json", true, true, true, false, false);
    simulation.getOutput().dumpWorkflowExecutionJSON(&workflow, "workflow_data.json", false);
    //simulation.getOutput().dumpWorkflowGraphJSON(&workflow, "workflow_graph.json");

    return 0;
}

#include <iostream>
#include <iomanip>
#include <wrench.h>

#include "ActivityWMS.h"
#include "ActivityScheduler.h"

int main(int argc, char** argv) {

    wrench::Simulation simulation;
    simulation.init(&argc, argv);

    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <xml paltform file> <workflow file>" << std::endl;
        exit(1);
    }

    char *platform_file = argv[1];
    char *workflow_file = argv[2];

    // load workflow
    wrench::Workflow workflow;
    workflow.loadFromDAX(workflow_file, "1000Gf");

    // read and instantiate the platform
    simulation.instantiatePlatform(platform_file);

    std::vector<std::string> hostname_list = simulation.getHostnameList();

    // set up a single storage service
    std::string storage_host = hostname_list[0];
    wrench::StorageService *storage_service = simulation.add(new wrench::SimpleStorageService(storage_host, 10000000000000.0));

    // set up multihost multicore compute service
    std::string compute_service_host = hostname_list[0];
    std::string compute_host_1 = hostname_list[1];
    std::string compute_host_2 = hostname_list[2];

    std::set<std::tuple<std::string, unsigned long, double>> compute_resources = {std::make_tuple(compute_host_1, wrench::ComputeService::ALL_CORES, wrench::ComputeService::ALL_RAM),
                                                                                          /*std::make_tuple(compute_host_2, wrench::ComputeService::ALL_CORES, wrench::ComputeService::ALL_RAM)*/};

    wrench::ComputeService *compute_service = simulation.add(new wrench::MultihostMulticoreComputeService(
                compute_service_host,
                compute_resources,
                0, {}, {}
            ));

    // set up the WMS
    std::string wms_host = hostname_list[2];
    wrench::WMS *wms = simulation.add(new wrench::ActivityWMS(std::unique_ptr<wrench::ActivityScheduler> (new wrench::ActivityScheduler(storage_service)),
            nullptr, {compute_service}, {storage_service}, wms_host));

    wms->addWorkflow(&workflow);

    // set up file registry service
    std::string file_registry_service_host = hostname_list[2];
    simulation.add(new wrench::FileRegistryService(file_registry_service_host));

    // stage the input files
    std::map<std::string, wrench::WorkflowFile *> input_files = workflow.getInputFiles();
    simulation.stageFiles(input_files, storage_service);

    // launch the simulation
    simulation.launch();
}




#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

#include <wrench.h>
#include <nlohmann/json.hpp>

#include "ActivityWMS.h"
#include "ActivityScheduler.h"

int main(int argc, char** argv) {

    wrench::Simulation simulation;
    simulation.init(&argc, argv);

    char *platform_file = argv[1];
    char *workflow_file = argv[2];

    // load workflow
    wrench::Workflow workflow;
    workflow.loadFromDAX(workflow_file, "1000Gf");

    // read and instantiate the platform
    simulation.instantiatePlatform(platform_file);

    std::string my_lab_computer_edu("my_lab_computer.edu");
    std::string hpc_edu("hpc.edu");
    std::string storage_db_edu("storage_db.edu");

    // storage service on storage_db_edu
    wrench::StorageService *storage_db_edu_storage_service = simulation.add(new wrench::SimpleStorageService(storage_db_edu, 10000000000000.0));
    wrench::StorageService *hpc_edu_storage_service = simulation.add(new wrench::SimpleStorageService(hpc_edu, 10000000000000.0));
    std::set<wrench::StorageService *> storage_services = {storage_db_edu_storage_service, hpc_edu_storage_service};

    // compute service on hpc_edu
    std::set<std::tuple<std::string, unsigned long, double>> compute_resources = {std::make_tuple(hpc_edu, wrench::ComputeService::ALL_CORES, wrench::ComputeService::ALL_RAM)};

    wrench::ComputeService *compute_service = simulation.add(new wrench::MultihostMulticoreComputeService(
                hpc_edu,
                compute_resources,
                0, {}, {}
            ));

    // WMS on my_lab_computer_edu
    wrench::WMS *wms = simulation.add(new wrench::ActivityWMS(std::unique_ptr<wrench::ActivityScheduler> (new wrench::ActivityScheduler(storage_services)),
            nullptr, {compute_service}, storage_services, my_lab_computer_edu));

    wms->addWorkflow(&workflow);

    // file registry service on storage_db_edu
    simulation.add(new wrench::FileRegistryService(storage_db_edu));

    // stage the input files
    std::map<std::string, wrench::WorkflowFile *> input_files = workflow.getInputFiles();
    simulation.stageFiles(input_files, storage_db_edu_storage_service);

    // launch the simulation
    simulation.launch();

    auto starts = simulation.getOutput().getTrace<wrench::SimulationTimestampTaskStart>();

    nlohmann::json workflow_data;
    std::string start("start");
    std::string end("end");

    for (auto &s : starts) {
        nlohmann::json task_data;
        wrench::WorkflowTask *task = s->getContent()->getTask();

        std::string task_id = task->getID();
        std::string execution_host = task->getExecutionHost();

        double task_start = task->getStartDate();

        double read_start = task->getReadInputStartDate();
        double read_end = task->getReadInputEndDate();

        double compute_start = task->getComputationStartDate();
        double compute_end = task->getComputationEndDate();

        double write_start = task->getWriteOutputStartDate();
        double write_end = task->getWriteOutputEndDate();

        double task_end = task->getEndDate();

        task_data["task_id"] = task_id;
        task_data["whole_task"] = {{start, task_start}, {end, task_end}};
        task_data["execution_host"] = execution_host;
        task_data["read"] = {{start, read_start}, {end, read_end}};
        task_data["compute"] = {{start, compute_start}, {end, compute_end}};
        task_data["write"] = {{start, write_start}, {end, write_end}};

        workflow_data.push_back(task_data);
    }

    std::string workflow_data_file_path = "workflow_data.json";
    std::ofstream output(workflow_data_file_path);
    output << std::setw(4) << workflow_data << std::endl;
    output.close();

}

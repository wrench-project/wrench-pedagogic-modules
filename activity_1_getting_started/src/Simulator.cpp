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

    std::vector<std::string> hostname_list = simulation.getHostnameList();
    std::string tremblay = hostname_list[2];
    std::string jupiter = hostname_list[1];
    std::string fafard = hostname_list[0];

    // storage service on Fafard
    wrench::StorageService *fafard_storage_service = simulation.add(new wrench::SimpleStorageService(fafard, 10000000000000.0));
    wrench::StorageService *jupiter_storage_service = simulation.add(new wrench::SimpleStorageService(jupiter, 10000000000000.0));
    std::set<wrench::StorageService *> storage_services = {fafard_storage_service, jupiter_storage_service};

    // compute service on Jupiter
    std::set<std::tuple<std::string, unsigned long, double>> compute_resources = {std::make_tuple(jupiter, wrench::ComputeService::ALL_CORES, wrench::ComputeService::ALL_RAM)};

    wrench::ComputeService *compute_service = simulation.add(new wrench::MultihostMulticoreComputeService(
                jupiter,
                compute_resources,
                0, {}, {}
            ));

    // WMS on Tremblay
    wrench::WMS *wms = simulation.add(new wrench::ActivityWMS(std::unique_ptr<wrench::ActivityScheduler> (new wrench::ActivityScheduler(storage_services)),
            nullptr, {compute_service}, storage_services, tremblay));

    wms->addWorkflow(&workflow);

    // file registry service on Fafard
    simulation.add(new wrench::FileRegistryService(fafard));

    // stage the input files
    std::map<std::string, wrench::WorkflowFile *> input_files = workflow.getInputFiles();
    simulation.stageFiles(input_files, fafard_storage_service);

    // launch the simulation
    simulation.launch();

    auto starts = simulation.getOutput().getTrace<wrench::SimulationTimestampTaskStart>();

    // write csv of task data
/*    std::ofstream csv;
    csv.open("workflow_data.csv");

    csv << "task_id,execution_host,task_start,read_start,read_end,compute_start,compute_end,write_start,write_end,task_end" << std::endl;*/

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

/*        csv << task_id << ",";
        csv << execution_host << ",";
        csv << read_start << ",";
        csv << read_end << ",";
        csv << compute_start << ",";
        csv << compute_end << ",";
        csv << write_start << ",";
        csv << write_end << ",";
        csv << task_end << std::endl;*/
    }
/*    csv.close();*/

    std::ofstream output("/home/wrench/workflow_data.json");
    output << std::setw(4) << workflow_data << std::endl;
    output.close();

}

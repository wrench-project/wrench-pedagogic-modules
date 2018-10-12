#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

#include <wrench.h>
#include <nlohmann/json.hpp>
#include <pugixml.hpp>

#include "ActivityWMS.h"
#include "ActivityScheduler.h"

/**
 * @brief Generate the workflow for activity 1
 * @description Fork-Join
 */
void generateWorkflow(wrench::Workflow *workflow) {

    const double TFLOP = 1000.0 * 1000.0 * 1000.0 * 1000.0;
    const double MB    = 1000.0 * 1000.0;

    wrench::WorkflowTask *task0 = workflow->addTask("task0", 50 * TFLOP, 1, 1, 1.0, 0);
    task0->addInputFile(workflow->addFile("task0::0.in", 700 * MB));
    task0->addOutputFile(workflow->addFile("task0::0.out", 100 * MB));
    task0->addOutputFile(workflow->addFile("task0::1.out", 200 * MB));
    task0->addOutputFile(workflow->addFile("task0::2.out", 400 * MB));

    wrench::WorkflowTask *task1 = workflow->addTask("task1", 40 * TFLOP, 1, 1, 1.0, 0);
    task1->addInputFile(workflow->getFileByID("task0::0.out"));
    task1->addOutputFile(workflow->addFile("task1::0.out", 120 * MB));

    wrench::WorkflowTask *task2 = workflow->addTask("task2", 35 * TFLOP, 1, 1, 1.0 , 0);
    task2->addInputFile(workflow->getFileByID("task0::1.out"));
    task2->addOutputFile(workflow->addFile("task2::0.out", 200 * MB));

    wrench::WorkflowTask *task3 = workflow->addTask("task3", 20 * TFLOP, 1, 1, 1.0, 0);
    task3->addInputFile(workflow->getFileByID("task0::2.out"));
    task3->addOutputFile(workflow->addFile("task3::0.out", 200 * MB));

    wrench::WorkflowTask *task4 = workflow->addTask("task4", 10 * TFLOP, 1, 1, 1.0, 0);
    task4->addInputFile(workflow->getFileByID("task1::0.out"));
    task4->addInputFile(workflow->getFileByID("task2::0.out"));
    task4->addInputFile(workflow->getFileByID("task3::0.out"));
    task4->addOutputFile(workflow->addFile("task4::0.out", 100 * MB));
}

/**
 * @brief Generate the platform file for activity 1
 * @param platform_file_path: path to write the file to
 * @param link_bandwidth: desired bandwidth in MBps
 */
void generatePlatform(std::string platform_file_path, int bandwidth) {
    if (platform_file_path.empty()) {
        throw std::invalid_argument("generatePlatform() platform_file_path cannot be empty");
    }

    if (bandwidth < 1 ) {
        throw std::invalid_argument("generatePlatform() bandwidth must be greater than 1");
    }

    std::string xml_string = "<?xml version='1.0'?>\n"
                      "<!DOCTYPE platform SYSTEM \"http://simgrid.gforge.inria.fr/simgrid/simgrid.dtd\">\n"
                      "<platform version=\"4.1\">\n"
                      "   <zone id=\"AS0\" routing=\"Full\">\n"
                      "       <host id=\"my_lab_computer.edu\" speed=\"1000Gf\" core=\"1\"/>\n"
                      "       <host id=\"hpc.edu\" speed=\"1000Gf\" core=\"1\"/>\n"
                      "       <host id=\"storage_db.edu\" speed=\"1000Gf\" core=\"1\"/>\n"

                      "       <link id=\"1\" bandwidth=\"10MBps\" latency=\"10us\"/>\n"

                      "       <route src=\"my_lab_computer.edu\" dst=\"hpc.edu\">\n"
                      "           <link_ctn id=\"1\"/>\n"
                      "       </route>\n"

                      "       <route src=\"storage_db.edu\" dst=\"my_lab_computer.edu\">\n"
                      "           <link_ctn id=\"1\"/>\n"
                      "       </route>\n"

                      "       <route src=\"storage_db.edu\" dst=\"hpc.edu\">\n"
                      "           <link_ctn id=\"1\"/>\n"
                      "       </route>\n"
                      "    </zone>\n"
                      "</platform>";

    pugi::xml_document xml_doc;

    if (xml_doc.load_string(xml_string.c_str(), pugi::parse_doctype)) {

        pugi::xml_node link = xml_doc.child("platform").child("zone").child("link");
        link.attribute("bandwidth").set_value(std::string(std::to_string(bandwidth) + "MBps").c_str());
        xml_doc.save_file(platform_file_path.c_str());

    } else {
        throw std::runtime_error("something went wrong with parsing xml string");
    }
}

int main(int argc, char** argv) {

    wrench::Simulation simulation;
    simulation.init(&argc, argv);

    int LINK_BANDWIDTH;

    try {
        if (argc != 2) {
            throw std::invalid_argument("bad args");
        }

        LINK_BANDWIDTH = std::stoi(std::string(argv[1]));

        if (LINK_BANDWIDTH < 1) {
            std::cerr << "Bandwidth must be greater than 0 MBps";
            throw std::invalid_argument("invalid bandwidth");
        }
    } catch(std::invalid_argument &e) {
        std::cerr << "Usage: activity_1_simulator <link_bandwidth>" << std::endl;
        std::cerr << "    link_bandwidth: measured as MBps" << std::endl;
        return 1;
    }

    // load workflow
    wrench::Workflow workflow;
    generateWorkflow(&workflow);

    std::string platform_file_path = "/tmp/platform.xml";
    generatePlatform(platform_file_path, LINK_BANDWIDTH);

    // read and instantiate the platform
    simulation.instantiatePlatform(platform_file_path);

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

    simulation.getOutput().dumpWorkflowExecutionJSON(&workflow, "workflow_data.json");
    simulation.getOutput().dumpWorkflowGraphJSON(&workflow, "workflow_graph.json");
}

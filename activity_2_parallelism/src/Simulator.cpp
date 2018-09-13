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
#include "./WorkflowToJSON.cpp"

// looks correct based on the visualization
void generateTaskJoinWorkflow(wrench::Workflow *workflow, int num_tasks_to_join, bool requiresMemory) {

    // WorkflowTask specifications
    const double               FLOPS = 60 * 1000.0 * 1000.0 * 1000.0 * 1000.0;
    const unsigned long    MIN_CORES = 1;
    const unsigned long    MAX_CORES = 1;
    const double PARALLEL_EFFICIENCY = 1.0;
    const double  MEMORY_REQUIREMENT = requiresMemory ? 16.0 * 1000.0 * 1000.0 * 1000.0 : 0.0;

    // WorkflowFile size
    const double FILE_SIZE = 100;

    // add final task
    auto final_task = workflow->addTask("task" + std::to_string(num_tasks_to_join), FLOPS, MIN_CORES, MAX_CORES, PARALLEL_EFFICIENCY, MEMORY_REQUIREMENT);

    // create number of desired tasks to join into one
    for (int i = 0; i < num_tasks_to_join; ++i) {
        std::string task_id("task" + std::to_string(i));
        auto current_task = workflow->addTask("task" + std::to_string(i), FLOPS, MIN_CORES, MAX_CORES, PARALLEL_EFFICIENCY, MEMORY_REQUIREMENT);
        current_task->addInputFile(workflow->addFile(task_id + ".in", FILE_SIZE));

        auto output_file = workflow->addFile(task_id + ".out", FILE_SIZE);
        current_task->addOutputFile(output_file);
        final_task->addInputFile(output_file);
        workflow->addControlDependency(current_task, final_task);
    }

    final_task->addOutputFile(workflow->addFile("task" + std::to_string(num_tasks_to_join) + ".out", FILE_SIZE));
}

// cant change host attributes programatically at the moment so will use pugixml to set num_nodes and num_cores for our cluster
void generatePlatformWithHPCSpecs(std::string src_platform_file, std::string dst_platform_file, int num_nodes, int num_cores) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(src_platform_file.c_str(), pugi::parse_doctype);

    if (result) {
        pugi::xml_node hpc_cluster = doc.child("platform").child("zone").child("cluster");

        hpc_cluster.attribute("radical").set_value(std::string("0-" + std::to_string(num_nodes)).c_str());
        hpc_cluster.attribute("core").set_value(num_cores);

        doc.save_file(dst_platform_file.c_str());
    } else {
        throw std::runtime_error("pugi had some problems with the xml");
    }
}

int main(int argc, char** argv) {

    wrench::Simulation simulation;
    simulation.init(&argc, argv);

    char *platform_file_input = argv[1];
    char *num_nodes_input     = argv[2];
    char *num_cores_input     = argv[3];
    char *num_task_join_input = argv[4];
    std::string task_memory_required_input  = std::string(argv[5]);

    const int MAX_NODES         = 32;
    const int MAX_CORES         = 64;
    const int MAX_TASKS_TO_JOIN = 50;

    int  NUM_HPC_NODES;
    int  NUM_CORES_PER_HPC_NODE;
    int  NUM_TASKS_TO_JOIN;
    bool TASK_MEMORY_REQUIRED;
    std::string MODIFIED_PLATFORM("./platform_files/modified_platform.xml");

    try {

        if (argc != 6) {
            throw std::invalid_argument("bad args");
        }

        NUM_HPC_NODES = std::stoi(std::string(num_nodes_input));

        if (NUM_HPC_NODES < 1 || NUM_HPC_NODES > MAX_NODES) {
            std::cerr << "Invalid number of HPC execution nodes. Enter a value in the range [1, " + std::to_string(MAX_NODES) + "]" << std::endl;
            throw std::invalid_argument("invalid number of execution nodes");
        }

        NUM_CORES_PER_HPC_NODE = std::stoi(std::string(num_cores_input));

        if (NUM_CORES_PER_HPC_NODE < 1 || NUM_CORES_PER_HPC_NODE > MAX_CORES) {
            std::cerr << "Invalid number of cores per HPC node. Enter a value in the range [1, " + std::to_string(MAX_CORES) + "]" << std::endl;
            throw std::invalid_argument("invalid number of cores per hpc node");
        }

        NUM_TASKS_TO_JOIN = std::stoi(std::string(num_task_join_input));

        if (NUM_TASKS_TO_JOIN < 2 || NUM_TASKS_TO_JOIN > MAX_TASKS_TO_JOIN) {
            std::cerr << "Invalid number of tasks to join. Enter a value in the range [2, " + std::to_string(MAX_TASKS_TO_JOIN) + "]" << std::endl;
            throw std::invalid_argument("invalid number of tasks to join");
        }

        if (task_memory_required_input == "RAM_REQUIRED") {
            TASK_MEMORY_REQUIRED = true;
        } else if (task_memory_required_input == "RAM_NOT_REQUIRED") {
            TASK_MEMORY_REQUIRED = false;
        } else {
            std::cerr << "Invalid memory requirement. Enter RAM_REQUIRED or RAM_NOT_REQUIRED" << std::endl;
            throw std::invalid_argument("invalid memory requirement");
        }

    } catch(std::invalid_argument &e) {
        std::cerr << "Usage: activity_2_simulator <platform_file> <num_hpc_nodes> <num_hpc_cores_per_node> <num_tasks_to_join> <task_memory_requirement>" << std::endl;
        std::cerr << "   platform_file: path to the xml platform file" << std::endl;
        std::cerr << "   num_hpc_nodes: number of execution nodes in the range [1, " + std::to_string(MAX_NODES) + "]" << std::endl;
        std::cerr << "   num_hpc_cores_per_node: number of cores per node in the range [1, " + std::to_string(MAX_CORES) + "]" << std::endl;
        std::cerr << "   num_tasks_to_join: the number of independent tasks to join in the range [2, " + std::to_string(MAX_TASKS_TO_JOIN) + "]" << std::endl;
        std::cerr << "   task_memory_requirement: RAM_REQUIRED or RAM_NOT_REQUIRED" << std::endl;
        return 1;
    }

    // create workflow
    wrench::Workflow workflow;
    generateTaskJoinWorkflow(&workflow, NUM_TASKS_TO_JOIN, TASK_MEMORY_REQUIRED);

    // read and instantiate the platform with the desired HPC specifications
    generatePlatformWithHPCSpecs(platform_file_input, MODIFIED_PLATFORM, NUM_HPC_NODES, NUM_CORES_PER_HPC_NODE);
    simulation.instantiatePlatform(MODIFIED_PLATFORM);

    // get all the hosts in the cluster zone
    simgrid::s4u::Engine *simgrid_engine = simgrid::s4u::Engine::get_instance();
    simgrid::s4u::NetZone *hpc_net_zone = simgrid_engine->netzone_by_name_or_null("hpc.edu");
    std::vector<simgrid::s4u::Host *> hpc_nodes = hpc_net_zone->get_all_hosts();

    // order nodes by the number postfixed to its name just so I can print them and use them in order if needed
    std::string hpc_node_name_format("hpc.edu/node_XXX");
    std::string::size_type index_of_node_number = hpc_node_name_format.find('_') + 1;

    std::sort(hpc_nodes.begin(), hpc_nodes.end(), [&index_of_node_number] (const simgrid::s4u::Host *lhs, const simgrid::s4u::Host *rhs) {
        return std::stoi(lhs->get_name().substr(index_of_node_number)) < std::stoi(rhs->get_name().substr(index_of_node_number));
    });

    #ifdef DEBUG
    std::cerr << "\n************** DEBUG INFO **************" << std::endl;
    std::cerr << "\nhpc.edu configuration" << std::endl;
    std::cerr << "-----------------------------" << std::endl;
    std::cerr << std::setw(18) << std::left  << "node"
        << std::setw(7) << std::left << "cores"
        << std::setw(14) << std::left << "ram" << std::endl;

    for (const auto &node : hpc_nodes) {
        std::cerr << std::setw(18) << std::left << node->get_name()
            << std::setw(7) << std::left << node->get_core_count()
            << std::setw(14) << std::left << node->get_property("ram") << std::endl;
    }

    std::cerr << "\nworkflow configuration" << std::endl;
    std::cerr << "----------------------" << std::endl;

    auto some_task = workflow.getTasks().at(0);

    std::cerr << std::setw(8) << std::left << "taskID"
        << std::setw(9) << std::left << "parents"
        << std::setw(11) << std::left << "children"
        << std::setw(15) << std::left << "input files"
        << std::setw(17) << std::left << "output files" << std::endl;

    for (const auto &task : workflow.getTasks()) {
        std::cerr << std::setw(8) << std::left << task->getID()
                  << std::setw(9) << std::left << task->getNumberOfParents()
                  << std::setw(11) << std::left << task->getNumberOfChildren()
                  << std::setw(15) << std::left << task->getInputFiles().size()
                  << std::setw(17) << std::left << task->getOutputFiles().size() << std::endl;
    }

    std::cerr << "\ntasks configuration" << std::endl;
    std::cerr << "-------------------" << std::endl;
    std::cerr << std::setw(15) << std::left << "computation: " << some_task->getFlops() << std::endl;
    std::cerr << std::setw(15) << std::left << "max cores: " << some_task->getMaxNumCores() << std::endl;
    std::cerr << std::setw(15) << std::left << "memory usage: " << some_task->getMemoryRequirement() << std::endl;

    auto some_file = *(some_task->getInputFiles().begin());

    std::cerr << "\nfile configuration" << std::endl;
    std::cerr << "------------------" << std::endl;
    std::cerr << "size: " << some_file->getSize() << std::endl;

    std::cerr << "\n****************************************\n" << std::endl;
    #endif

   const std::string REMOTE_STORAGE_HOST("storage_db.edu");
   const std::string WMS_HOST("my_lab_computer.edu");

   // storage service
   const double STORAGE_CAPACITY = 10.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0;
   wrench::StorageService *remote_storage_service = simulation.add(
            new wrench::SimpleStorageService(REMOTE_STORAGE_HOST, STORAGE_CAPACITY));



   // compute service
   std::set<std::tuple<std::string, unsigned long, double>> compute_nodes;

   // compute resources will be all the nodes in the cluster except node_0
   for (auto node = hpc_nodes.begin() + 1; node != hpc_nodes.end(); ++node) {
       compute_nodes.insert(std::make_tuple((*node)->get_name(), wrench::ComputeService::ALL_CORES, wrench::ComputeService::ALL_RAM));
   }

   wrench::ComputeService *compute_service = simulation.add(
           new wrench::MultihostMulticoreComputeService(
                   (*hpc_nodes.begin())->get_name(), compute_nodes, STORAGE_CAPACITY, {}, {}
                   )
           );

   // wms
   wrench::WMS *wms = simulation.add(new wrench::ActivityWMS(std::unique_ptr<wrench::ActivityScheduler>(
           new wrench::ActivityScheduler(remote_storage_service)), nullptr, {compute_service}, {remote_storage_service}, WMS_HOST
           ));

   wms->addWorkflow(&workflow);

   // file registry service
   simulation.add(new wrench::FileRegistryService(REMOTE_STORAGE_HOST));

   // stage input files
   simulation.stageFiles(workflow.getInputFiles(), remote_storage_service);

   simulation.launch();

   return 0;
}

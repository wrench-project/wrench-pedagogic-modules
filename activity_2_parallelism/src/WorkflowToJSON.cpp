#include <nlohmann/json.hpp>
#include <wrench.h>
#include <fstream>

// this is just a test for now, later you might need to compute the level of each task on the dag
void dumpWorkflowToJSON(wrench::Workflow * workflow, std::string file_path) {
    if (workflow == nullptr || file_path.empty()) {
        throw std::invalid_argument("Simulation::dumpTaskDataJSON() requires a valid workflow and file_path");
    }

    nlohmann::json nodes;
    nlohmann::json links;

    const std::string NODE_TYPE("type");
    const std::string ID("id");
    const std::string WORKFLOW_TASK("workflowTask");
    const std::string WORKFLOW_FILE("workflowFile");

    const std::string SOURCE("source");
    const std::string TARGET("target");

    // add the task nodes
    for (const auto &task : workflow->getTasks()) {
        nodes.push_back({{NODE_TYPE, WORKFLOW_TASK}, {ID, task->getID()}});
    }

    // add the file nodes
    for (const auto &file : workflow->getFiles()) {
        nodes.push_back({{NODE_TYPE, WORKFLOW_FILE}, {ID, file->getID()}});
    }

    // add the links
    for (const auto &task : workflow->getTasks()) {

        // create links between input files (if any) and the current task
        for (const auto &input_file : task->getInputFiles()) {
            links.push_back({{SOURCE, input_file->getID()}, {TARGET, task->getID()}});
        }

        bool hasOutputFiles = (task->getOutputFiles().size() != 0) ? true : false;
        bool hasChildren = (workflow->getTaskChildren(task).size() != 0) ? true : false;

        // create the links between output files (if any) and the current task
        // if none exist BUT the task has children, then create the links from the current task to the children
        if (hasOutputFiles) {
            for (const auto & output_file : task->getOutputFiles()) {
                links.push_back({{SOURCE, task->getID()}, {TARGET, output_file->getID()}});
            }
        } else if (hasChildren) {
            for (const auto & child : workflow->getTaskChildren(task)) {
                links.push_back({{SOURCE, task->getID()}, {TARGET, child->getID()}});
            }
        }
    }

    nlohmann::json workflow_task_graph;
    workflow_task_graph["nodes"] = nodes;
    workflow_task_graph["links"] = links;


    std::ofstream output(file_path);
    output << std::setw(4) << workflow_task_graph << std::endl;
    output.close();
}
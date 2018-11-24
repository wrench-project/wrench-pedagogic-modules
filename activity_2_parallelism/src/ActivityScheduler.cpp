
#include "ActivityScheduler.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(simple_wms_scheduler, "Log category for Simple WMS Scheduler");


namespace wrench {

    typedef struct ComputeResource {
        std::string hostname;
        unsigned long num_idle_cores;
        double available_ram;
    } ComputeResource;

    ActivityScheduler::ActivityScheduler(std::map<std::string, StorageService *> storage_services) : StandardJobScheduler(), storage_services(storage_services) {

    }

    void ActivityScheduler::scheduleTasks(const std::set<wrench::ComputeService *> &compute_services,
                                          const std::vector<WorkflowTask *> &ready_tasks) {

        TerminalOutput::setThisProcessLoggingColor(TerminalOutput::Color::COLOR_BLUE);
        ComputeService *compute_service = *compute_services.begin();

        auto idle_core_counts = compute_service->getNumIdleCores();
        auto ram_capacities = compute_service->getMemoryCapacity();



        // combining core counts and ram capacities together
        std::vector<ComputeResource> available_resources;
        for (const auto &host : idle_core_counts) {
            if (host.second > 0) {
                available_resources.push_back({host.first,
                                               host.second,
                                               ram_capacities.at(host.first)});
            }
        }

        // add tasks to a "tasks_to_submit" vector until core and or ram requirements cannot be met
        std::vector<WorkflowTask *> tasks_to_submit;
        std::map<std::string, std::string> service_specific_args;
        for (const auto &task : ready_tasks) {
            for (auto &resource : available_resources) {
                if (task->getMaxNumCores() <= resource.num_idle_cores && task->getMemoryRequirement() <= resource.available_ram) {
                    tasks_to_submit.push_back(task);
                    service_specific_args[task->getID()] = resource.hostname + ":" + std::to_string(task->getMaxNumCores());

                    resource.num_idle_cores -= task->getMaxNumCores();
                    resource.available_ram -= task->getMemoryRequirement();
                    break;
                }
            }
        }

        // specify file locations for tasks that will be submitted
        std::map<WorkflowFile *, StorageService *> file_locations;
        for (const auto &task : tasks_to_submit) {

            bool taskHasChildren = (task->getNumberOfChildren() != 0) ? true : false;

            // initial input files should be read from the remote storage service
            // files "in between" should be read from the local storage service (same host as the baremetal CS)
            for (const auto &file : task->getInputFiles()) {
                if (taskHasChildren) {
                    file_locations.insert(std::make_pair(file, storage_services.at("storage_db.edu")));
                } else {
                    file_locations.insert(std::make_pair(file, storage_services.at("hpc.edu/node_0")));
                }
            }

            // the final output file should be written to the remote storage service
            // files "in between" should be written to the local storage service (same host as teh baremetal CS)
            for (const auto &file: task->getOutputFiles()) {
                if (not taskHasChildren) {
                    file_locations.insert(std::make_pair(file, storage_services.at("storage_db.edu")));
                } else {
                    file_locations.insert(std::make_pair(file, storage_services.at("hpc.edu/node_0")));
                }
            }
        }

        WorkflowJob *job = (WorkflowJob *) this->getJobManager()->createStandardJob(tasks_to_submit, file_locations);
        this->getJobManager()->submitJob(job, compute_service, service_specific_args);

    }
}

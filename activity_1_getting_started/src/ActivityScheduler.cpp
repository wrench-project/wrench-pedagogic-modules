
#include "ActivityScheduler.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(simple_wms_scheduler, "Log category for Simple WMS Scheduler");


namespace wrench {

    ActivityScheduler::ActivityScheduler(std::set<wrench::StorageService *> storage_services) : StandardJobScheduler() {
        for (auto &s : storage_services) {
            if (s->getHostname() == "Fafard") {
                this->storage_services.insert(std::make_pair(s->getHostname(), s));
            } else if (s->getHostname() == "Jupiter") {
                this->storage_services.insert(std::make_pair(s->getHostname(), s));
            }
        }
    }

    void ActivityScheduler::scheduleTasks(const std::set<wrench::ComputeService *> &compute_services,
                                          const std::vector<WorkflowTask *> &tasks) {

        TerminalOutput::setThisProcessLoggingColor(TerminalOutput::Color::COLOR_BLUE);
        ComputeService *compute_service = *compute_services.begin();

        for (auto task : tasks) {

            std::map<WorkflowFile *, StorageService *> file_locations;

            #ifdef REMOTE_STORAGE
            for (auto f : task->getInputFiles()) {
                file_locations.insert(std::make_pair(f, storage_services["Fafard"]));
            }

            for (auto f : task->getOutputFiles()) {
                file_locations.insert(std::make_pair(f, storage_services["Fafard"]));
            }
            #endif

            #ifdef LOCAL_STORAGE
            for (auto f : task->getInputFiles()) {
                if (task->getNumberOfParents() == 0) { // if im the first task, all my inputs should be read from remote
                    file_locations.insert(std::make_pair(f, storage_services["Fafard"]));
                } else {
                    file_locations.insert(std::make_pair(f, storage_services["Jupiter"]));
                }
            }

            for (auto f : task->getOutputFiles()) {
                if (task->getNumberOfChildren() == 0) { // if im the last task, all my outputs should be written to remote
                    file_locations.insert(std::make_pair(f, storage_services["Fafard"]));
                }
                file_locations.insert(std::make_pair(f, storage_services["Jupiter"]));
            }
            #endif


            WRENCH_INFO("Submitting %s as a job to compute service on %s", task->getID().c_str(), compute_service->getHostname().c_str());
            WorkflowJob *job = (WorkflowJob *) this->getJobManager()->createStandardJob(task, file_locations);
            this->getJobManager()->submitJob(job, *compute_services.begin());
        }
    }
}

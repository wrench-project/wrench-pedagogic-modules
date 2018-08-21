
#include "ActivityScheduler.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(simple_wms_scheduler, "Log category for Simple WMS Scheduler");


namespace wrench {
    void ActivityScheduler::scheduleTasks(const std::set<ComputeService *> &compute_services,
                                         const std::vector<WorkflowTask *> &tasks) {

        TerminalOutput::setThisProcessLoggingColor(TerminalOutput::Color::COLOR_BLUE);
        wrench::ComputeService *compute_service = *compute_services.begin();

        for (auto task : tasks) {
            std::map<WorkflowFile *, StorageService *> file_locations;
            for (auto f : task->getInputFiles()) {
                file_locations.insert(std::make_pair(f, default_storage_service));
            }
            for (auto f : task->getOutputFiles()) {
                file_locations.insert(std::make_pair(f, default_storage_service));
            }

            WRENCH_INFO("Submitting %s as a job to compute service on %s", task->getID().c_str(), compute_service->getHostname().c_str());
            WorkflowJob *job = (WorkflowJob *) this->getJobManager()->createStandardJob(task, file_locations);
            this->getJobManager()->submitJob(job, *compute_services.begin());
        }
    }
}

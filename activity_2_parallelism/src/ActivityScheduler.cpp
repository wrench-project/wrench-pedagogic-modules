
#include "ActivityScheduler.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(simple_wms_scheduler, "Log category for Simple WMS Scheduler");


namespace wrench {

    ActivityScheduler::ActivityScheduler(StorageService *remote_storage_service) : StandardJobScheduler() {
        this->remote_storage_service = remote_storage_service;
    }

    void ActivityScheduler::scheduleTasks(const std::set<wrench::ComputeService *> &compute_services,
                                          const std::vector<WorkflowTask *> &tasks) {

        TerminalOutput::setThisProcessLoggingColor(TerminalOutput::Color::COLOR_BLUE);
        ComputeService *compute_service = *compute_services.begin();

        std::map<WorkflowFile *, StorageService *> file_locations;
        for (const auto &task : tasks) {

            bool taskHasChildren = (task->getNumberOfChildren() != 0) ? true : false;

            for (const auto &file : task->getInputFiles()) {
                if (taskHasChildren) {
                    file_locations.insert(std::make_pair(file, remote_storage_service));
                }
            }

            for (const auto &file: task->getOutputFiles()) {
                if (not taskHasChildren) {
                    file_locations.insert(std::make_pair(file, remote_storage_service));
                }
            }
        }

        WorkflowJob *job = (WorkflowJob *) this->getJobManager()->createStandardJob(tasks, file_locations);
        this->getJobManager()->submitJob(job, *compute_services.begin());

    }
}

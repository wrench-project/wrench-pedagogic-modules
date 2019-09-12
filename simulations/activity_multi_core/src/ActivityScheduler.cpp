
#include "ActivityScheduler.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(simple_wms_scheduler, "Log category for Simple WMS Scheduler");


namespace wrench {

    /**
     * @brief A struct representing a "Compute Node"
     */
    typedef struct ComputeResource {
        std::string hostname;
        unsigned long num_idle_cores;
    } ComputeResource;

    /**
     * @brief Constructor
     * @param storage_services: a map of hostname key to StorageService pointer
     */
    ActivityScheduler::ActivityScheduler() : StandardJobScheduler() {

    }

    void ActivityScheduler::scheduleTasks(const std::set<std::shared_ptr<ComputeService>> &compute_services,
                                          const std::vector<WorkflowTask *> &ready_tasks) {

        TerminalOutput::setThisProcessLoggingColor(TerminalOutput::Color::COLOR_BLUE);
        auto compute_service = *compute_services.begin();

        auto idle_core_counts = compute_service->getPerHostNumIdleCores()["the_host"];

        for (auto const &ready_task : ready_tasks) {
            if (idle_core_counts == 0) {
                break;
            }
            auto job = this->getJobManager()->createStandardJob(ready_task, {});
            this->getJobManager()->submitJob(job, compute_service, {});
            idle_core_counts--;
        }

    }
}

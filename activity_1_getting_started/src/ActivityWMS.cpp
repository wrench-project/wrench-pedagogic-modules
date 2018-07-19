
#include "ActivityWMS.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(simple_wms, "Log category for Simple WMS");

namespace wrench {

    ActivityWMS::ActivityWMS(std::unique_ptr <StandardJobScheduler> standard_job_scheduler,
                             std::unique_ptr<PilotJobScheduler> pilot_job_scheduler,
                             const std::set<ComputeService *> &compute_services,
                             const std::set<StorageService *> &storage_services,
                             const std::string &hostname) : WMS (
                                     std::move(standard_job_scheduler),
                                     std::move(pilot_job_scheduler),
                                     compute_services,
                                     storage_services,
                                     {}, nullptr,
                                     hostname,
                                     "activity"
                                     ) {}


    int ActivityWMS::main() {
        TerminalOutput::setThisProcessLoggingColor(TerminalOutput::Color::COLOR_GREEN);

        WRENCH_INFO("Starting on host %s listening on mailbox_name %s",
                    S4U_Simulation::getHostName().c_str(),
                    this->mailbox_name.c_str());
        WRENCH_INFO("About to execute a workflow with %lu tasks", this->getWorkflow()->getNumberOfTasks());

        // Create a job manager
        this->job_manager = this->createJobManager();
        
        while (true) {

            // Get the ready tasks
            std::vector<WorkflowTask *> ready_tasks = this->getWorkflow()->getReadyTasks();

            // Get the available compute services
            std::set<ComputeService *> compute_services = this->getAvailableComputeServices();

            // Run ready tasks with defined scheduler implementation
            WRENCH_INFO("Scheduling tasks...");
            this->getStandardJobScheduler()->scheduleTasks(
                    this->getAvailableComputeServices(),
                    ready_tasks);

            // Wait for a workflow execution event, and process it
            try {
                this->waitForAndProcessNextEvent();
            } catch (WorkflowExecutionException &e) {
                WRENCH_INFO("Error while getting next execution event (%s)... ignoring and trying again",
                            (e.getCause()->toString().c_str()));
                continue;
            }
            if (this->abort || this->getWorkflow()->isDone()) {
                break;
            }
        }

        WRENCH_INFO("--------------------------------------------------------");
        if (this->getWorkflow()->isDone()) {
            WRENCH_INFO("Workflow execution is complete!");
        } else {
            WRENCH_INFO("Workflow execution is incomplete!");
        }

        WRENCH_INFO("Simple WMS Daemon started on host %s terminating", S4U_Simulation::getHostName().c_str());

        this->job_manager.reset();

        return 0;
    }
}
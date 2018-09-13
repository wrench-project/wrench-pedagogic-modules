
#include "ActivityWMS.h"
#include <algorithm>

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
                                     "activity2"
                                     ) {}


    int ActivityWMS::main() {
        TerminalOutput::setThisProcessLoggingColor(TerminalOutput::Color::COLOR_MAGENTA);

        WRENCH_INFO("Starting on host %s listening on mailbox_name %s",
                    S4U_Simulation::getHostName().c_str(),
                    this->mailbox_name.c_str());
        WRENCH_INFO("About to execute a workflow with %lu tasks", this->getWorkflow()->getNumberOfTasks());

        // Create a job manager
        this->job_manager = this->createJobManager();
        
        while (true) {

            // Get all the tasks and SORT them by taskID
            std::vector<WorkflowTask *> tasks = this->getWorkflow()->getTasks();

            std::sort(tasks.begin(), tasks.end(), [ ] (WorkflowTask *lhs, WorkflowTask *rhs) {
                return lhs->getID() < rhs->getID();
            });

            // Get the available compute services, in this case only one for activity1
            std::set<ComputeService *> compute_services = this->getAvailableComputeServices();

            // Run ready tasks with defined scheduler implementation
            this->getStandardJobScheduler()->scheduleTasks(
                    this->getAvailableComputeServices(),
                    tasks);

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
        TerminalOutput::setThisProcessLoggingColor(TerminalOutput::Color::COLOR_MAGENTA);

        WRENCH_INFO("--------------------------------------------------------");
        if (this->getWorkflow()->isDone()) {
            WRENCH_INFO("Workflow execution completed in %f seconds!", wrench::S4U_Simulation::getClock());
        } else {
            WRENCH_INFO("Workflow execution is incomplete!");
        }

        this->job_manager.reset();

        return 0;
    }

    void ActivityWMS::processEventStandardJobCompletion(std::unique_ptr<wrench::StandardJobCompletedEvent> event) {
        auto standard_job = event->standard_job;
        TerminalOutput::setThisProcessLoggingColor(TerminalOutput::Color::COLOR_RED);
        WRENCH_INFO("Notified that a standard job with %d tasks has completed", standard_job->getTasks().size());
    }
}

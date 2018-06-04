//
// Created by ryan on 6/1/18.
//

#include "ActivityScheduler.h"

namespace wrench {
    void ActivityScheduler::scheduleTasks(const std::set<ComputeService *> &compute_services,
                                         const std::vector<WorkflowTask *> &tasks) {

        for (auto task : tasks) {
            std::map<WorkflowFile *, StorageService *> file_locations;
            for (auto f : task->getInputFiles()) {
                file_locations.insert(std::make_pair(f, default_storage_service));
            }
            for (auto f : task->getOutputFiles()) {
                file_locations.insert(std::make_pair(f, default_storage_service));
            }

            WorkflowJob *job = (WorkflowJob *) this->getJobManager()->createStandardJob(task, file_locations);
            this->getJobManager()->submitJob(job, *compute_services.begin());
        }
    }

}

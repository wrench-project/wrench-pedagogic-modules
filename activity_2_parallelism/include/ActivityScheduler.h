#ifndef WRENCH_ACTIVITY_SCHEDULER_H
#define WRENCH_ACTIVITY_SCHEDULER_H

#include <wrench-dev.h>

namespace wrench {
    class Simulation;

    class ActivityScheduler : public StandardJobScheduler {

    public:
        void scheduleTasks(const std::set<ComputeService *> &compute_services,
                           const std::vector<WorkflowTask *> &tasks);


        ActivityScheduler(StorageService *remote_storage_service) ;

    private:
       StorageService * remote_storage_service;
    };
}

#endif 

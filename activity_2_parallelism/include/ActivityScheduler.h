#ifndef WRENCH_ACTIVITY_SCHEDULER_H
#define WRENCH_ACTIVITY_SCHEDULER_H

#include <wrench-dev.h>

namespace wrench {
    class Simulation;

    class ActivityScheduler : public StandardJobScheduler {

    public:
        void scheduleTasks(const std::set<ComputeService *> &compute_services,
                           const std::vector<WorkflowTask *> &ready_tasks);


        ActivityScheduler(std::map<std::string, StorageService *> storage_services) ;

    private:
       std::map<std::string, StorageService *> storage_services;
    };
}

#endif 

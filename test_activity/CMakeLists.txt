cmake_minimum_required(VERSION 3.2)
message(STATUS "Cmake version ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}")

project(ACTIVITY_1)

add_definitions("-Wall -Wno-unused-variable -Wno-unused-private-field")

set(CMAKE_CXX_STANDARD 11)

# include directories for dependencies and WRENCH libraries
include_directories(src/ include/  /usr/local/include /usr/local/include/wrench)

# source files
set(SOURCE_FILES
    include/SimpleWMS.h
    src/SimpleWMS.cpp
    src/SimpleSimulatorCloud.cpp
    src/optimizations/dynamic/FailureDynamicClustering.h
    src/optimizations/dynamic/FailureDynamicClustering.cpp
    src/optimizations/static/SimplePipelineClustering.h
    src/optimizations/static/SimplePipelineClustering.cpp
    src/scheduler/BatchStandardJobScheduler.h
    src/scheduler/BatchStandardJobScheduler.cpp
    src/scheduler/CloudStandardJobScheduler.h
    src/scheduler/CloudStandardJobScheduler.cpp
    src/scheduler/MaxMinStandardJobScheduler.h
    src/scheduler/MaxMinStandardJobScheduler.cpp
    src/scheduler/MinMinStandardJobScheduler.h
    src/scheduler/MinMinStandardJobScheduler.cpp
    src/scheduler/pilot_job/CriticalPathPilotJobScheduler.h
    src/scheduler/pilot_job/CriticalPathPilotJobScheduler.cpp
    )

# wrench library and dependencies
find_library(WRENCH_LIBRARY NAMES libwrench.a wrench)
find_library(SIMGRID_LIBRARY NAMES simgrid)
find_library(PUGIXML_LIBRARY NAMES pugixml)
find_library(LEMON_LIBRARY NAMES emon)

# generating the cloud example
add_executable(wrench-simple-wms-cloud ${SOURCE_FILES})
target_link_libraries(wrench-simple-wms-cloud 
                        ${WRENCH_LIBRARY}
                        ${SIMGRID_LIBRARY}
                        ${PUGIXML_LIBRARY}
                        ${LEMON_LIBRARY}
                        )
install(TARGETS wrench-simple-wms-cloud DESTINATION bin)

cmake_minimum_required(VERSION 3.2)
message(STATUS "Cmake version ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}")

project(ACTIVITY_1)

add_definitions("-Wall -Wno-unused-variable -Wno-unused-private-field")

set(CMAKE_CXX_STANDARD 11)

# include directories for dependencies and WRENCH libraries
include_directories(src/ include/  /usr/local/include /usr/local/include/wrench)

# source files
set(SOURCE_FILES
        include/ActivityWMS.h
        src/ActivityWMS.cpp
        include/ActivityScheduler.h
        src/ActivityScheduler.cpp
        )

# wrench library and dependencies
find_library(WRENCH_LIBRARY NAMES libwrench.a wrench)
find_library(SIMGRID_LIBRARY NAMES simgrid)
find_library(PUGIXML_LIBRARY NAMES pugixml)
find_library(LEMON_LIBRARY NAMES emon)

add_executable(activity_simulator ${SOURCE_FILES} src/Simulator1.cpp)
target_link_libraries(activity_simulator1
        ${WRENCH_LIBRARY}
        ${SIMGRID_LIBRARY}
        ${PUGIXML_LIBRARY}
        ${LEMON_LIBRARY}
        )

add_executable(activity_simulator2 ${SOURCE_FILES} src/Simulator2.cpp)
target_link_libraries(activity_simulator2
        ${WRENCH_LIBRARY}
        ${SIMGRID_LIBRARY}
        ${PUGIXML_LIBRARY}
        ${LEMON_LIBRARY}
        )

#include <iostream>
#include <fstream>
#include <iomanip>
#include <pugixml.hpp>

#include "ActivityWMS.h"

void generateWorkflow(wrench::Workflow *workflow, int num_files, int file_size_in_MB) {
    for (int i = 0; i < num_files; ++i) {
        workflow->addFile(("file_" + std::to_string(i)), file_size_in_MB * 1000.0 * 1000.0);
    }
}

void generatePlatform(std::string platform_file_path, unsigned long bandwidth) {
    if (platform_file_path.empty()) {
        throw std::invalid_argument("generateSingleLinkPlatform() platform_file_path cannot be empty");
    }

    if (bandwidth < 1) {
        throw std::invalid_argument("generateSingleLinkPlatform() bandwidth must be at least 1 MBps");
    }

    std::string xml_string = "<?xml version='1.0'?>\n"
                             "<!DOCTYPE platform SYSTEM \"http://simgrid.gforge.inria.fr/simgrid/simgrid.dtd\">\n"
                             "<platform version=\"4.1\">\n"
                             "   <zone id=\"AS0\" routing=\"Full\">\n"
                             "       <host id=\"host1\" speed=\"1000Gf\" core=\"1\"/>\n"
                             "       <host id=\"host2\" speed=\"1000Gf\" core=\"1\"/>\n"

                             "       <link id=\"link1\" bandwidth=\"100MBps\" latency=\"1us\"/>\n"
                             "       <link id=\"link2\" bandwidth=\"100MBps\" latency=\"1us\"/>\n"
                             "       <link id=\"link3\" bandwidth=\"100MBps\" latency=\"1us\"/>\n"

                             "       <route src=\"host1\" dst=\"host2\">\n"
                             "           <link_ctn id=\"link1\"/>\n"
                             "           <link_ctn id=\"link2\"/>\n"
                             "           <link_ctn id=\"link3\"/>\n"
                             "       </route>\n"
                             "    </zone>\n"
                             "</platform>";

    pugi::xml_document xml_doc;

    if (xml_doc.load_string(xml_string.c_str(), pugi::parse_doctype)) {

        pugi::xml_node zone = xml_doc.child("platform").child("zone");
        pugi::xml_node middle_link = zone.find_child_by_attribute("link", "id", "link2");

        middle_link.attribute("bandwidth").set_value(std::string(std::to_string(bandwidth) + "MBps").c_str());

        xml_doc.save_file(platform_file_path.c_str());

    } else {
        throw std::runtime_error("something went wrong with parsing xml string");
    }
}

int main(int argc, char **argv) {
    wrench::Simulation simulation;
    simulation.init(&argc, argv);

    const int MAX_NUM_FILES = 100;
    const int MAX_FILE_SIZE = 1000;
    const int MAX_BANDWIDTH = 1000;

    int NUM_FILES;
    int FILE_SIZE;
    int BANDWIDTH;

    try {
        if (argc != 4) {
            throw std::invalid_argument("bad args");
        }

        NUM_FILES = std::stoi(std::string(argv[1]));

        if (NUM_FILES < 1 || NUM_FILES > MAX_NUM_FILES) {
            std::cerr << "Invalid number of files. Enter a value in the range [1, " + std::to_string(MAX_NUM_FILES) + "]" << std::endl;
            throw std::invalid_argument("invalid number of files");
        }

        FILE_SIZE = std::stoi(std::string(argv[2]));

        if (FILE_SIZE < 1 || FILE_SIZE > MAX_FILE_SIZE) {
            std::cerr << "Invalid file size. Enter a file size in the range [1, " + std::to_string(MAX_FILE_SIZE) + "] MB" << std::endl;
            throw std::invalid_argument("invalid file size");
        }

        BANDWIDTH = std::stoi(std::string(argv[3]));

        if (BANDWIDTH < 1 || BANDWIDTH > MAX_BANDWIDTH) {
            std::cerr << "Invalid bandwidth. Enter a value in the range [1, " + std::to_string(MAX_BANDWIDTH) + "] MBps" << std::endl;
            throw std::invalid_argument("invalid bandwidth");
        }
    } catch (std::invalid_argument &e) {
        std::cerr << "Usage: activity_0_simulator <num_files> <file_size> <bandwidth>" << std::endl;
        std::cerr << "    num_files: number of files to send concurrently from host1 to host2 in the range [1, " + std::to_string(MAX_NUM_FILES) + "]" << std::endl;
        std::cerr << "    file_size: the size of each file, a value in the range of [1, " + std::to_string(MAX_FILE_SIZE) + "] MB" << std::endl;
        std::cerr << "    bandwidth: the bandwidth of the middle link, a value in the range of [1, " + std::to_string(MAX_BANDWIDTH) + "] MBps" << std::endl;
        return 1;
    }

    std::string platform_file_path = "/tmp/platform.xml";
    generatePlatform(platform_file_path, BANDWIDTH);
    simulation.instantiatePlatform(platform_file_path);

    // two storage services
    const double STORAGE_CAPACITY = MAX_FILE_SIZE * MAX_NUM_FILES * 1000.0 * 1000.0;
    wrench::StorageService *storage_service_1 = simulation.add(
            new wrench::SimpleStorageService("host1", STORAGE_CAPACITY)
            );

    wrench::StorageService *storage_service_2 = simulation.add(
            new wrench::SimpleStorageService("host2", STORAGE_CAPACITY)
    );

    // wms
    wrench::WMS *wms = simulation.add(new wrench::ActivityWMS({storage_service_1, storage_service_2}, "host1"));

    wrench::Workflow workflow;
    generateWorkflow(&workflow, NUM_FILES, FILE_SIZE);
    wms->addWorkflow(&workflow);

    // file registry service
    simulation.add(new wrench::FileRegistryService("host1"));

    for (const auto &file : workflow.getFiles()) {
        simulation.stageFile(file, storage_service_1);
    }

    simulation.launch();

    auto file_copy_starts = simulation.getOutput().getTrace<wrench::SimulationTimestampFileCopyStart>();

/*    std::string file_name_format("file_XXX");
    std::string::size_type index_of_file_number = file_name_format.find('_') + 1;

    std::sort(file_copy_starts.begin(), file_copy_starts.end(), [&index_of_file_number] (wrench::SimulationTimestamp<wrench::SimulationTimestampFileCopyStart> *lhs, wrench::SimulationTimestamp<wrench::SimulationTimestampFileCopyStart> *rhs) {
       return std::stoi(lhs->getContent()->getFile()->getID().substr(index_of_file_number)) < std::stoi(rhs->getContent()->getFile()->getID().substr(index_of_file_number));
    });*/

    for (auto const &file_copy : file_copy_starts) {
        std::cerr << std::setw(8) << file_copy->getContent()->getFile()->getID() << std::setw(20) << " start: " + std::to_string(file_copy->getDate()) << std::setw(20) << " end: " + std::to_string(file_copy->getContent()->getEndpoint()->getDate()) << std::endl;
    }

}

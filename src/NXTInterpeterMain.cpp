#include <cmath>
#include <iostream>
#include <thread>
#include <sstream>

#include "Execution/ExecutionContext.h"
#include "Environment/Environment.h"
#include "Environment/Simulation.h"
#include "Robot/SingleRobotNetworkInterface.h"

#include "Environment/Map.h"

#define TIME_MULTIPLIER 1 // x

std::string default_map =
        "25;25 12;12"
        "* * * * * * * * * * * * * * * * * * * * * * * * *"
        "* o o o o o o o o o o o o o o o o o o o o o o o *"
        "* o o o o o o o o o o o o o o o o o o o o o o o *"
        "* o o o o o o o o o o o o o o o o o o o o o o o *"
        "* o o o o o o o o o o o o o o o o o o o o o o o *"
        "* o o o o o o o o o o o o o o o o o o o o o o o *"
        "* o o o o o o o o o o o o o o o o o o o o o o o *"
        "* o o o o o o o o o o o o o o o o o o o o o o o *"
        "* o o o o o o o o o o o o o o o o o o o o o o o *"
        "* o o o o o o o o o o o o o o o o o o o o o o o *"
        "* o o o o o o o o o o o o o o o o o o o o o o o *"
        "* o o o o o o o o o o o o o o o o o o o o o o o *"
        "* o o o o o o o o o o o o o o o o o o o o o o o *"
        "* o o o o o o o o o o o o o o o o o o o o o o o *"
        "* o o o o o o o o o o o o o o o o o o o o o o o *"
        "* o o o o o o o o o o o o o o o o o o o o o o o *"
        "* o o o o o o o o o o o o o o o o o o o o o o o *"
        "* o o o o o o o o o o o o o o o o o o o o o o o *"
        "* o o o o o o o o o o o o o o o o o o o o o o o *"
        "* o o o o o o o o o o o o o o o o o o o o o o o *"
        "* o o o o o o o o o o o o o o o o o o o o o o o *"
        "* o o o o o o o o o o o o o o o o o o o o o o o *"
        "* o o o o o o o o o o o o o o o o o o o o o o o *"
        "* o o o o o o o o o o o o o o o o o o o o o o o *"
        "* * * * * * * * * * * * * * * * * * * * * * * * *";

void printUsageAndExit(const std::string& pname)
{
    std::cout << "Usage: "  << pname << " [options] inputfile" << std::endl << std::endl;
    std::cout << "Execute RXE file" << std::endl << std::endl;
    std::cout << "   -h: print this help and exit" << std::endl;
    std::cout << "   -g: debug output" << std::endl;
    std::cout << "   -s <factor>: speedup factor (default=" << TIME_MULTIPLIER << ")" << std::endl;
    std::cout << "   -m <file.map>: map file" << std::endl;
    std::cout << "   -c <file.csv>: csv output file (position and angle)" << std::endl;

    exit(0);
}

void printRobotInfo(const Robot* robot) {
    std::cout << "position of the robot is "
    << "x=" << robot->getPosition()[3][0]
    << ";z=" <<  robot->getPosition()[3][2]
    << ";theta=" << robot->getYaw() * 180. / M_PI;
}

void outRobotInfo(unsigned tick, const Robot* robot, std::ostream& out) {
    out << tick
        << ";" << robot->getPosition()[3][0]
        << ";" <<  robot->getPosition()[3][2]
        << ";" << robot->getYaw() * 180. / M_PI << "\n";
}

int main(int argc, char *argv[]) {
    if (argc < 2) printUsageAndExit(argv[0]);

    bool debug = false;
    char* filename = nullptr;
    char* map_filename = nullptr;
    char* csv_filename = nullptr;
    int time_multiplier = TIME_MULTIPLIER;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-g") == 0)
            debug = true;
        else if (strcmp(argv[i], "-h") == 0) {
            printUsageAndExit(argv[0]);
        } else if(strcmp(argv[i], "-m") == 0) {
            if (argc <= i + 1) {
                std::cout << "argument -m: expected filename" << std::endl;
                printUsageAndExit(argv[0]);
            }

            map_filename = argv[i + 1];
            i += 1;
        } else if(strcmp(argv[i], "-s") == 0) {
            if (argc <= i + 1) {
                std::cout << "argument -s: expected factor" << std::endl;
                printUsageAndExit(argv[0]);
            }

            time_multiplier = atoi(argv[i+1]);
            i += 1;
        } else if(strcmp(argv[i], "-c") == 0) {
            if (argc <= i + 1) {
                std::cout << "argument -c: expected file" << std::endl;
                printUsageAndExit(argv[0]);
            }

            csv_filename = argv[i+1];
            i += 1;
        }
        else
            filename = argv[i];
    }

    if (filename == nullptr)
        printUsageAndExit(argv[0]);

    // create execution context
    ExecutionContext* context;

    try {
        context = new ExecutionContext(filename);
    } catch (std::runtime_error& e ) {
        std::cout << "An error happened while opening the file (" << filename << "): " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    // create simulation (out of map)
    MapParser* map_parser;
    Simulation* simulation;

    try {
        if(map_filename == nullptr) {
            auto stream = std::stringstream(default_map);
            map_parser = new MapParser(stream);
        } else {
            auto stream = std::ifstream(map_filename);

            if(!stream.is_open())
                throw std::runtime_error(std::string("cannot open map file ") + map_filename);

            map_parser = new MapParser(stream);
        }

        simulation = map_parser->createSimulation();
    } catch (std::runtime_error& e) {
        std::cout << "An error happened while parsing the map: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    // Setup robot through its "network interface"
    NetworkInterface* robotInterface;

    try {
        robotInterface = new SingleRobotNetworkInterface(simulation, map_parser->getRobotX(), map_parser->getRobotZ());
        context->setNetworkInterface(robotInterface);
    } catch (std::runtime_error& e){
        std::cout << "An error happened while setting the robot: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    auto* robot = robotInterface->getLocalRobot();

    if(debug) {
        std::cout << "initial ";
        printRobotInfo(robot);
        std::cout << std::endl;
    }

    // open csv file
    std::ofstream csv_out;

    if(csv_filename) {
        csv_out.open(csv_filename);
        if (!csv_out.is_open()) {
            std::cout << "cannot open CSV output " << csv_filename << std::endl;
            return EXIT_FAILURE;
        }
    }

    // Simulate
    bool running = true;

    if(debug)
        std::cout << "-- start (at ticks=" << context->getTicks() << ")" << std::endl;

    if (csv_out.is_open())
        outRobotInfo(context->getTicks(), robot, csv_out);

    while (running) {
        running = context->runForTime(float(time_multiplier) * 0.001f);

        if (csv_out.is_open())
            outRobotInfo(context->getTicks(), robot, csv_out);

        if(running) {
            simulation->update(float(time_multiplier) * 0.001f);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    if (debug) {
        std::cout << "-- end (at ticks=" << context->getTicks() << ")" << std::endl;

        std::cout << "final ";
        printRobotInfo(robot);
        std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}

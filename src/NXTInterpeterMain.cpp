#include <cmath>
#include <iostream>
#include <thread>
#include <sstream>

#include "Execution/ExecutionContext.h"
#include "Environment/Environment.h"
#include "Environment/Simulation.h"
#include "Robot/SingleRobotNetworkInterface.h"

#include "Environment/Map.h"

#define EXEC_FRACTION 0.1f // [%]
#define TOTAL_ROUND .001f // [s]

std::string default_map =
        "25;25 5;5"
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
    std::cout << "Execute RXE file" << std::endl;
    std::cout << "Usage: "  << pname << "[-m mapfile] [-g] inputfile" << std::endl;
    exit(0);
}

void printRobotInfo(const Robot* robot) {
    std::cout << "position of the robot is "
    << "x=" << robot->getPosition()[3][0]
    << ";z=" <<  robot->getPosition()[3][2]
    << ";theta=" << robot->getYaw() * 180. / M_PI;
}

int main(int argc, char *argv[]) {
    if (argc < 2) printUsageAndExit(argv[0]);

    bool debug = false;
    char* filename = nullptr;
    char* map_filename = nullptr;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-g") == 0)
            debug = true;
        else if(strcmp(argv[i], "-m") == 0) {
            if (argc <= i + 1) {
                std::cout << "argument -m: expected filename" << std::endl;
                printUsageAndExit(argv[0]);
            }

            map_filename = argv[i + 1];
            i += 2;
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
        std::cout << "An error happened while opening the file: " << e.what() << std::endl;
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
        std::cout << ".note initial ";
        printRobotInfo(robot);
        std::cout << std::endl;
    }

    // Simulate
    float run_time = EXEC_FRACTION * TOTAL_ROUND;
    int nperiod = 0;
    bool running = true;

    if(debug)
        std::cout << "-- start (at t=" << context->getTick() << " ms)" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    while (running) {
        running = context->runForTime(run_time);
        auto now = std::chrono::high_resolution_clock::now();
        simulation->update(float(std::chrono::duration_cast<std::chrono::microseconds>(now - start).count()) / 1000000.f);
        start = now;
        nperiod += 1;

        if(running)
            std::this_thread::sleep_for(std::chrono::microseconds (int((TOTAL_ROUND - run_time) * 1000000.f)));
    }

    if (debug) {
        std::cout << "-- end (at t=" << context->getTick() << " ms, nperiod=" << nperiod << ")" << std::endl;

        std::cout << ".note final ";
        printRobotInfo(robot);
        std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}

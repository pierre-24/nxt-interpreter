#include <cmath>
#include <iostream>
#include <thread>

#include "Execution/ExecutionContext.h"
#include "Environment/Environment.h"
#include "Environment/Simulation.h"
#include "Robot/SingleRobotNetworkInterface.h"

#define EXEC_FRACTION 0.01f // [%]
#define TOTAL_ROUND .01f // [s]

void printUsageAndExit(const std::string& pname)
{
    std::cout << "Execute RXE file" << std::endl;
    std::cout << "Usage: "  << pname << " inputfile" << std::endl;
    exit(0);
}

void printRobotInfo(const Robot* robot) {
    std::cout << "position of the robot is x=" << robot->getPosition()[3][0] << ";y=" <<  robot->getPosition()[3][2]<< ";theta=" << std::acos(robot->getPosition()[0][0]) / M_PI * 180.f;
}

int main(int argc, char *argv[]) {
    if (argc != 2) printUsageAndExit(argv[0]);

    ExecutionContext* context;

    std::cout << ".setup context and system" << std::endl;

    try {
        context = new ExecutionContext(argv[1]);
    } catch (std::runtime_error& e ) {
        std::cout << "An error happened while opening the file: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    // setup environment
    std::cout << ".setup environment" << std::endl;
    auto* simulation = new Simulation(new Environment(25, 25, 1.0f, 0.75f)); // TODO: change size and stuffs

    // Setup robot through its "network interface"
    std::cout << ".setup robot" << std::endl;
    auto* robotInterface = new SingleRobotNetworkInterface(simulation);
    context->setNetworkInterface(robotInterface);

    auto* robot = robotInterface->getLocalRobot();

    std::cout << ".note initial ";
    printRobotInfo(robot);
    std::cout << std::endl;

    // simulate
    float run_time = EXEC_FRACTION * TOTAL_ROUND;
    int nperiod = 0;
    bool running = true;
    unsigned long start, end;

    std::cout << ".note period is " << int(TOTAL_ROUND * 1000) << " ms" << std::endl;

    std::cout << "-- start (at t=" << context->getTick() << " ms)" << std::endl;

    while (running) {
        start = context->getTick();
        running = context->runForTime(run_time);
        simulation->update(TOTAL_ROUND);
        end = context->getTick();
        nperiod++;
        std::this_thread::sleep_for(std::chrono::milliseconds(int(TOTAL_ROUND * 1000.f) - int(end - start)));
    }

    std::cout << "-- end (at t=" << context->getTick() << " ms, nperiod=" << nperiod << ")" << std::endl;

    std::cout << ".note final ";
    printRobotInfo(robot);
    std::cout << std::endl;

    return EXIT_SUCCESS;
}

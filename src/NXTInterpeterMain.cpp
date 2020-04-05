#include <cmath>
#include <iostream>
#include <thread>

#include "Execution/ExecutionContext.h"
#include "Environment/Environment.h"
#include "Environment/Simulation.h"
#include "Robot/SingleRobotNetworkInterface.h"

#define EXEC_FRACTION 0.1f // [%]
#define TOTAL_ROUND .001f // [s]

void printUsageAndExit(const std::string& pname)
{
    std::cout << "Execute RXE file" << std::endl;
    std::cout << "Usage: "  << pname << " inputfile" << std::endl;
    exit(0);
}

void printRobotInfo(const Robot* robot) {
    std::cout << "position of the robot is "
    << "x=" << robot->getPosition()[3][0]
    << ";y=" <<  robot->getPosition()[3][2]
    // TODO: is this assuming an opposite convention w.r.t. the trigonometric one?
    << ";theta=" << std::acos(robot->getPosition()[0][0]) / M_PI * 180. * ((robot->getPosition()[2][0] > 0)? -1. : 1.);
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

    std::cout << ".note period is (about) " << int(TOTAL_ROUND * 1000) << " ms" << std::endl;

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

    std::cout << "-- end (at t=" << context->getTick() << " ms, nperiod=" << nperiod << ")" << std::endl;

    std::cout << ".note final ";
    printRobotInfo(robot);
    std::cout << std::endl;

    return EXIT_SUCCESS;
}

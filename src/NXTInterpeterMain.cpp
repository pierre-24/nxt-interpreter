#include <iostream>

#include "Execution/ExecutionContext.h"

void printUsageAndExit(const std::string& pname)
{
    std::cout << "Execute RXE file" << std::endl;
    std::cout << "Usage: "  << pname << " inputfile" << std::endl;
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 2) printUsageAndExit(argv[0]);

    std::cout << "-- Starting interpreter" << std::endl;

    ExecutionContext* context;

    try {
        context = new ExecutionContext(argv[1]);
    } catch (std::runtime_error& e ) {
        std::cout << "An error happened while opening the file: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    // TODO: set "network interface"
    // TODO: setup Environment (?)
    // TODO: run until the end (?)

    std::cout << "-- Ending interpreter" << std::endl;
    return EXIT_SUCCESS;
}

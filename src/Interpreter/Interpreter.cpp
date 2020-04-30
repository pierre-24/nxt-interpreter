//
// Created by pierre on 06/04/2020.
//

#include <chrono>
#include <iostream>

#include "Interpreter.h"
#include "InterpreterThread.h"
#include "../System/System.h"
#include "../Execution/RXEFile.h"

Interpreter::Interpreter(const RXEFile *aFile,System *aSystem): file(aFile), system(aSystem)
{
    scheduleClump(0); // schedule clump 0
}

bool Interpreter::runForTicks(unsigned int nticks) noexcept(false) {
    if (threads.empty())
        return false;

    int minTicksPerThread = int(nticks / threads.size()); // fair share of computation time, does not account for thread creation or sleeping thread
    auto it = threads.begin();

    while (it != threads.end()) {
        InterpreterThread* th = *it;

        if (th->done()) {
            it = threads.erase(it);
        } else {
            if (th->waitingUntilTime() <= system->getTimeSinceStart()) {
                unsigned start = system->getTicks();
                bool running = true;
                while (running && (system->getTicks() - start) < minTicksPerThread) {
                    running = th->step();
                    system->tickIt();
                }
            } else {
                system->tickIt(minTicksPerThread);
            }

            ++it;
        }
    }

    return !threads.empty();
}

void Interpreter::scheduleClump(unsigned clump) {
    threads.push_back(new InterpreterThread(clump, this, system));
}

void Interpreter::scheduleDependantClumps(unsigned clump, unsigned start, unsigned end) {
    for(unsigned i = start; i <= end; i++) {
        scheduleClump(file->getDependentsForClump(clump)[i]);
    }
}
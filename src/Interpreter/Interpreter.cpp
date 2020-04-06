//
// Created by pierre on 06/04/2020.
//

#include <chrono>

#include "Interpreter.h"
#include "InterpreterThread.h"
#include "../System/System.h"
#include "../Execution/RXEFile.h"

Interpreter::Interpreter(const RXEFile *aFile, VMMemory *aMemory, System *aSystem): file(aFile), memory(aMemory), system(aSystem)
{
    scheduleClump(0); // schedule clump 0
}

template<typename Clock>
float timeSinceStart(const std::chrono::time_point<Clock> start) {
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count()/1000000.0f;
}

bool Interpreter::runForTime(float mintime) noexcept(false) {
    if (threads.empty())
        return false;

    float minTimePerThread = mintime / threads.size(); // fair share of computation time, does not account for thread creation or sleeping thread
    auto it = threads.begin();

    while (it != threads.end()) {
        InterpreterThread* th = *it;

        if (th->done()) {
            it = threads.erase(it);
        } else {
            if (th->waitingUntilTick() <= system->getTick()) {
                auto thread_start = std::chrono::high_resolution_clock::now();
                bool running = true;
                while (running && timeSinceStart(thread_start) < minTimePerThread)
                    running = th->step();
            }

            ++it;
        }
    }

    return !threads.empty();
}

void Interpreter::scheduleClump(unsigned clump) {
    threads.push_back(new InterpreterThread(clump, this, file, memory, system));
}

void Interpreter::scheduleDependantClumps(unsigned clump, unsigned start, unsigned end) {
    for(unsigned i = start; i <= end; i++) {
        scheduleClump(file->getDependentsForClump(clump)[i]);
    }
}
//
// Created by pierre on 06/04/2020.
//

#include <list>

class RXEFile;
class System;
class VMMemory;
class InterpreterThread;

/*!
 * @abstract Interpreter object, implement a basic threading mechanism.
 * @discussion Every scheduled clump run on a different virtual thread.
 * When `runForTime()` is called, every thread is executed sequentially once during an equal period of time
 * (except, of course, if it is waiting).
 */
class Interpreter {
    const RXEFile *file;
    VMMemory *memory;
    System *system;

    std::list<InterpreterThread*> threads;

public:
    /*!
     * @abstract Constructs an Interpreter.
     * @discussion Execution always starts at the first clump, based on the
     * order in the clump data array in the RXE file. The interpreter does not
     * take ownership of any of the objects passed into it, they all have to be
     * deleted manually after the interpreter is deleted.
     * @param file The RXE file to execute.
     * @param memory A memory object, which has to have been created with the
     * same RXE file.
     * @param system The System interface used for IO and syscalls.
     */
    Interpreter(const RXEFile *file, VMMemory *memory, System *system);

    /*!
	 * @abstract Executes code for the specified number of seconds.
	 * Return false if there is nothing more to execute.
	 * @discussion Executes Lego bytecode until at least the time specified here
	 * has run out. There is no more code to execute, it exits immediately and return false,
     * while for lengthy operations, it can return after significantly more than the
	 * minimal time.
	 * @param mintime The time to execute for.
	 */
    bool runForTime(float mintime);

    /*!
     * @abstract schedule the clump
     * @param clump: clump to schedule
     */
    void scheduleClump(unsigned clump);

    /*!
     * @abstract for a given clump, schedule the dependant clumps, from start to end (included)
     * @param clump: the clump
     * @param start: first dependent clump to schedule
     * @param end: last dependent clump to schedule
     */
    void scheduleDependantClumps(unsigned clump, unsigned start, unsigned end);
};

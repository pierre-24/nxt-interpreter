/*
 *  InterpreterThread.h
 *  mindstormssimulation
 *
 *  Created by Torsten Kammer on 25.04.10
 *  Copyright 2010 RWTH Aachen University All rights reserved.
 *
 *  Modified by Pierre Beaujean on 06.04.20
 *
 */

#include <vector>
#include <cstdint>

class RXEFile;
class System;
class VMMemory;
class Interpreter;

/*!
 * @abstract Runs the bytecode.
 * @coclass RXEFile Contains the memory start information and the code that is
 * being executed.
 * @coclass System Calls into system for IO and communicating with the robot that
 * this program is running on.
 * @coclass VMMemory Manages the data the program is operating on.
 * @discussion The InterpreterThread ultimately executes all bytecode. It has only one
 * interesting public method, step, which executes a single operation. However,
 * it has a large number of private methods, one for each operation that exists.
 * Not all of these are implemented, many simply print out their name and that
 * they are not yet supported.
 */
class InterpreterThread
{
	const RXEFile *file;
	VMMemory *memory;
	System *system;
	Interpreter* interpreter; // for scheduling
	
	std::vector<unsigned> stack;
	unsigned instruction; // note: only valid when `isTerminated` is false
	unsigned currentClump;
	
	// Required for waiting
	unsigned waitUntil;

	// done
	bool isTerminated;
	
	// Helpers used internally by other ops
	bool compare(unsigned mode, int a, int b);
	void configureOutputForPort(unsigned port, unsigned numParams, const uint16_t *params);
	
	// Math Instructions
	void op_add(unsigned flags, const uint16_t *params);
	void op_sub(unsigned flags, const uint16_t *params);
	void op_neg(unsigned flags, const uint16_t *params);
	void op_mul(unsigned flags, const uint16_t *params);
	void op_div(unsigned flags, const uint16_t *params);
	void op_mod(unsigned flags, const uint16_t *params);
	void op_sqrt(unsigned flags, const uint16_t *params); // not implemented
	void op_abs(unsigned flags, const uint16_t *params);
	
	// Logical Instructions
	void op_and(unsigned flags, const uint16_t *params); // not implemented
	void op_or(unsigned flags, const uint16_t *params); // not implemented
	void op_xor(unsigned flags, const uint16_t *params); // not implemented
	void op_not(unsigned flags, const uint16_t *params);
	
	// Compare Instructions
	void op_cmp(unsigned flags, const uint16_t *params);
	void op_tst(unsigned flags, const uint16_t *params);
	
	// Data manipulation Instructions
	void op_index(unsigned flags, const uint16_t *params);
	void op_replace(unsigned flags, const uint16_t *params); // not implemented
	void op_arrsize(unsigned flags, const uint16_t *params);
	void op_arrbuild(unsigned flags, const uint16_t *params);
	void op_arrsubset(unsigned flags, const uint16_t *params); // not implemented
	void op_arrinit(unsigned flags, const uint16_t *params);
	void op_mov(unsigned flags, const uint16_t *params);
	void op_set(unsigned flags, const uint16_t *params);
	void op_flatten(unsigned flags, const uint16_t *params); // not implemented
	void op_unflatten(unsigned flags, const uint16_t *params); // not implemented
	void op_numtostring(unsigned flags, const uint16_t *params); // not implemented
	void op_stringtonum(unsigned flags, const uint16_t *params); // not implemented
	void op_strcat(unsigned flags, const uint16_t *params); // not implemented
	void op_strsubset(unsigned flags, const uint16_t *params); // not implemented
	void op_strtobytearr(unsigned flags, const uint16_t *params); // not implemented
	void op_bytearrtostr(unsigned flags, const uint16_t *params); // not implemented
	
	// Control flow Instructions
	void op_jmp(unsigned flags, const uint16_t *params);
	void op_brcmp(unsigned flags, const uint16_t *params);
	void op_brtst(unsigned flags, const uint16_t *params);
	void op_stop(unsigned flags, const uint16_t *params);
	void op_finclump(unsigned flags, const uint16_t *params);
	void op_finclumpimmed(unsigned flags, const uint16_t *params);
	void op_acquire(unsigned flags, const uint16_t *params); // not implemented
	void op_release(unsigned flags, const uint16_t *params); // not implemented
	void op_subcall(unsigned flags, const uint16_t *params);
	void op_subret(unsigned flags, const uint16_t *params);
	
	// System I/O instructions
	void op_syscall(unsigned flags, const uint16_t *params);
	void op_setin(unsigned flags, const uint16_t *params);
	void op_setout(unsigned flags, const uint16_t *params);
	void op_getin(unsigned flags, const uint16_t *params);
	void op_getout(unsigned flags, const uint16_t *params);
	void op_wait(unsigned flags, const uint16_t *params);
	void op_gettick(unsigned flags, const uint16_t *params);
	
public:
	/*!
	 * @abstract Constructs an InterpreterThread, to execute a given clump.
	 * @param clump: the clump that the thread must start to execute
	 * @param interpreter the parent interpreter
	 * @param file The RXE file to execute.
	 * @param memory A memory object, which has to have been created with the
	 * same RXE file.
	 * @param system The System interface used for IO and syscalls.
	 */
	InterpreterThread(unsigned clump, Interpreter* interpreter, const RXEFile *file, VMMemory *memory, System *system);
	
	/*!
	 * @abstract Executes a single operation. Return false if the thread is terminated
	 */
    bool step();
	
	/*!
	 * @abstract Tick at which the robot will next do something.
	 * @discussion If the robot is in wait mode, it will do nothing and no
	 * operation will be executed. With this, users can find out how long it
	 * will be until it operates again.
	 */
	unsigned waitingUntilTick() const { return waitUntil; }

	/*!
	 * @abstract Return true if the thread is terminated
	 */
	bool done() { return isTerminated; }
};
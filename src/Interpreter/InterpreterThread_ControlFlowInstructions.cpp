/*
 *  Interpreter_ControlFlowInstructions.cpp
 *  mindstormssimulation
 *
 *  Created by Torsten Kammer on 26.04.10
 *  Copyright 2010 RWTH Aachen University All rights reserved.
 *
 */

#include "InterpreterThread.h"
#include "Interpreter.h"

#include <iostream>

#include "../Execution/RXEFile.h"
#include "../System/VMMemory.h"

void InterpreterThread::op_jmp(unsigned flags, const uint16_t *params)
{
	// Parameters:
	// 0: offset (immediate, signed): Alter PC by this much.
	
	int16_t offset = int16_t(params[0]);
	instruction += (offset - 2);
	// Account for size of jmp instruction, as otherwise we will go right past it.
}

void InterpreterThread::op_brcmp(unsigned flags, const uint16_t *params)
{
	// Parameters:
	// 0: offset (immediate, signed): Alter PC by this much if comparison is true
	// 1: Source1, memory location
	// 2: Source2, memory location
	
	int a = memory->getScalarValue(params[1]);
	int b = memory->getScalarValue(params[2]);
	if (aggregatedComparisonBetweenScalarValues(flags, a, b))
	{
		int16_t offset = int16_t(params[0]);
		instruction += (offset - 4);
		// Account for size of instruction, as otherwise we will go right past it.
	}
}

void InterpreterThread::op_brtst(unsigned flags, const uint16_t *params)
{
	// Parameters:
	// 0: offset (immediate, signed): Alter PC by this much if comparison is true
	// 1: Source, memory location
	
	int a = memory->getScalarValue(params[1]);
	if (aggregatedComparisonBetweenScalarValues(flags, a, 0))
	{
		int16_t offset = int16_t(params[0]);
		instruction += (offset - 3);
		// Account for size of instruction, as otherwise we will go right past it.
	}
}

void InterpreterThread::op_stop(unsigned flags, const uint16_t *params)
{
	throw std::runtime_error("op_stop");
}

void InterpreterThread::op_finclump(unsigned flags, const uint16_t *params)
{
    // parameters:
    // 0: start, immediate. First clump to schedule
    // 1: end, immediate. Last clump to schedule

    isTerminated = true;

    if(params[0] != 0xffff) {
        interpreter->scheduleDependantClumps(currentClump, params[0], params[1]);
    }
}

void InterpreterThread::op_finclumpimmed(unsigned flags, const uint16_t *params)
{
    // parameters:
    // 0: clumpId, immediate. Clump to schedule

    isTerminated = true;
    interpreter->scheduleClump(params[0]);
}

void InterpreterThread::op_acquire(unsigned flags, const uint16_t *params)
{
	std::cout << "ignored acquire" << std::endl;
}

void InterpreterThread::op_release(unsigned flags, const uint16_t *params)
{
	std::cout << "ignored release" << std::endl;
}

void InterpreterThread::op_subcall(unsigned flags, const uint16_t *params)
{
	// Parameters:
	// 0: Subroutine, immediate. Clump to branch to.
	// 1: CallerID, memory. Store ID of this clump there.
	
	// Store clump to jump back to.
	memory->setScalarValue(params[1], currentClump);
	// Store instruction to jump back to
	stack.push_back(instruction);
	
	// Branch into new clump
	currentClump = params[0];
	instruction = memory->getFile()->getCodeStartForClump(currentClump);
}

void InterpreterThread::op_subret(unsigned flags, const uint16_t *params)
{
	// Parameters:
	// 0: CallerID, memory. Jump back to clump with this ID.
	
	// Find old clump and restore operation there.
	currentClump = memory->getScalarValue(params[0]);
	instruction = stack.back();
	stack.pop_back();
}

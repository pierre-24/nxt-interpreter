/*
 *  Interpreter_LogicalInstructions.cpp
 *  mindstormssimulation
 *
 *  Created by Torsten Kammer on 26.04.10
 *  Copyright 2010 RWTH Aachen University All rights reserved.
 *
 */

#include "InterpreterThread.h"

#include <iostream>

#include "../Execution/RXEFile.h"
#include "../System/VMMemory.h"

void InterpreterThread::op_and(unsigned flags, const uint16_t *params)
{
    // Params:
    // 0: Destination, memory location
    // 1: Source 1, memory location
    // 2: Source 2, memory location

    memory->setScalarValue(params[0], unsigned (memory->getScalarValue(params[1])) & unsigned (memory->getScalarValue(params[2])));
}

void InterpreterThread::op_or(unsigned flags, const uint16_t *params)
{
    // Params:
    // 0: Destination, memory location
    // 1: Source 1, memory location
    // 2: Source 2, memory location

    memory->setScalarValue(params[0], unsigned (memory->getScalarValue(params[1])) | unsigned (memory->getScalarValue(params[2])));
}

void InterpreterThread::op_xor(unsigned flags, const uint16_t *params)
{
    // Params:
    // 0: Destination, memory location
    // 1: Source 1, memory location
    // 2: Source 2, memory location

    memory->setScalarValue(params[0], unsigned (memory->getScalarValue(params[1])) ^ unsigned (memory->getScalarValue(params[2])));
}

void InterpreterThread::op_not(unsigned flags, const uint16_t *params)
{
	// Params:
	// 0: Destination, memory location
	// 1: A, memory location
	
	int a = memory->getScalarValue(params[1]);
	
	// Notice: This should be logical, not bitwise, per Lego documentation
	memory->setScalarValue(params[0], !a);
}

/*
 *  Interpreter_MathInstructions.cpp
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

void InterpreterThread::op_add(unsigned flags, const uint16_t *params)
{
	// Params:
	// 0: Destination, memory location
	// 1: A, memory location
	// 2: B, memory location
	
	int a = memory->getScalarValue(params[1]);
	int b = memory->getScalarValue(params[2]);
	
	memory->setScalarValue(params[0], a + b);
}

void InterpreterThread::op_sub(unsigned flags, const uint16_t *params)
{
	// Params:
	// 0: Destination, memory location
	// 1: A, memory location
	// 2: B, memory location
	
	int a = memory->getScalarValue(params[1]);
	int b = memory->getScalarValue(params[2]);
	
	memory->setScalarValue(params[0], a - b);
}

void InterpreterThread::op_neg(unsigned flags, const uint16_t *params)
{
	// Params:
	// 0: Destination, memory location
	// 1: A, memory location
	
	int a = memory->getScalarValue(params[1]);
	
	memory->setScalarValue(params[0], -a);
}

void InterpreterThread::op_mul(unsigned flags, const uint16_t *params)
{
	// Params:
	// 0: Destination, memory location
	// 1: A, memory location
	// 2: B, memory location
	
	int a = memory->getScalarValue(params[1]);
	int b = memory->getScalarValue(params[2]);
	
	memory->setScalarValue(params[0], a * b);
}

void InterpreterThread::op_div(unsigned flags, const uint16_t *params)
{
	// Params:
	// 0: Destination, memory location
	// 1: A, memory location
	// 2: B, memory location
	
	int a = memory->getScalarValue(params[1]);
	int b = memory->getScalarValue(params[2]);
	
	if (b == 0) memory->setScalarValue(params[0], 0);
	else memory->setScalarValue(params[0], a / b);
}

void InterpreterThread::op_mod(unsigned flags, const uint16_t *params)
{
	// Params:
	// 0: Destination, memory location
	// 1: A, memory location
	// 2: B, memory location
	
	int a = memory->getScalarValue(params[1]);
	int b = memory->getScalarValue(params[2]);
	
	if (b == 0) memory->setScalarValue(params[0], 0);
	else memory->setScalarValue(params[0], a % b);
}

void InterpreterThread::op_sqrt(unsigned flags, const uint16_t *params)
{
	std::cout << "ignored sqrt" << std::endl;
}

void InterpreterThread::op_abs(unsigned flags, const uint16_t *params)
{
	// std::cout << "ignored abs for now." << std::endl;
	int a = memory->getScalarValue(params[1]);
	memory->setScalarValue(params[0], abs(a));
}

/*
 *  Interpreter_CompareInstructions.cpp
 *  mindstormssimulation
 *
 *  Created by Torsten Kammer on 26.04.10
 *  Copyright 2010 RWTH Aachen University All rights reserved.
 *
 */

#include <iostream>
#include "InterpreterThread.h"

#include "../Execution/RXEFile.h"
#include "../System/VMMemory.h"

bool InterpreterThread::compare(unsigned mode, int a, int b) // TODO: to tune !!
{
	switch (mode)
	{
		case 0: // <
			return a < b;
		case 1: // >
			return a > b;
		case 2: // <=
			return a <= b;
		case 3: // >=
			return a >= b;
		case 4: // ==
			return a == b;
		case 5: // !=
			return a != b;
		default:
			throw std::runtime_error("Unknown compare mode");
	}
}

bool isAgregatedType(RXEFile::dstocType t) {
    return t == RXEFile::TC_ARRAY || t == RXEFile::TC_CLUSTER;
}

void InterpreterThread::op_cmp(unsigned flags, const uint16_t *params)
{
	// Params:
	// 0: Destination, memory location - store result here
	// 1: Source1, memory location
	// 2: Source2, memory location

	auto dest_type = file->getTypeAtDSTOCIndex(params[0]),
	    s1_type = file->getTypeAtDSTOCIndex(params[1]),
	    s2_type = file->getTypeAtDSTOCIndex(params[2]);

    if (!isAgregatedType(s1_type) && !isAgregatedType(s2_type)) {
        int a = memory->getScalarValue(params[1]);
        int b = memory->getScalarValue(params[2]);

        memory->setScalarValue(params[0], compare(flags, a, b));
    } else {
        if(!isAgregatedType(dest_type)) {
            if(s1_type == RXEFile::TC_ARRAY && s2_type == RXEFile::TC_ARRAY) { // element per element comparison
                std::cout << "op_cmp: compare arrays::" << memory->getArrayLength(params[1]) << "," << memory->getArrayLength(params[2]) << std::endl;
                bool result = memory->getArrayLength(params[1]) == memory->getArrayLength(params[2]);
                // TODO: not exactly. Anyway, I need a recursive call for that.
                memory->setScalarValue(params[0], result);
            } else {
                std::cout << "op_cmp: comparison of complex data not implemented" << std::endl;
            }
        } else {
            std::cout << "op_cmp: non-agregated comparison not implemented" << std::endl;
        }
    }
}

void InterpreterThread::op_tst(unsigned flags, const uint16_t *params)
{
	// Params:
	// 0: Destination, memory location - store result here
	// 1: Source, memory location
	int a = memory->getScalarValue(params[1]);
	memory->setScalarValue(params[0], compare(flags, a, 0));
}

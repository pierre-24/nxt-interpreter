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
#include "AggregatedTypeIterators.h"

bool InterpreterThread::aggregatedComparisonBetweenScalarValues(unsigned mode, int a, int b)
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

bool InterpreterThread::aggregatedComparisonBetweenScalarValueAndAggregated(unsigned mode, int32_t scalar, unsigned memoryLoc, bool isScalarLeft) {
    AggregatedTypeIterator* it = getAggregatedIterator(memory, memoryLoc);
    while (it->hasNext()) {
        bool result;
        if(memory->getFile()->isAggregatedType(it->elementType()))
            result = aggregatedComparisonBetweenScalarValueAndAggregated(mode, scalar, **it, isScalarLeft);
        else
            result = aggregatedComparisonBetweenScalarValues(mode, isScalarLeft? scalar: **it, isScalarLeft? **it : scalar);

        if(!result) {
            delete it;
            return false;
        }
        ++it;
    }

    delete it;
    return true;
}

bool InterpreterThread::aggregatedComparisonBetweenMemoryLocation(unsigned mode, unsigned mem1, unsigned mem2) {
    bool isLeftAggregated = memory->getFile()->isAggregatedType(mem1), isRightAggregated = memory->getFile()->isAggregatedType(mem2);

    if(!isLeftAggregated && !isRightAggregated) { // scalar comparison
        return aggregatedComparisonBetweenScalarValues(mode, memory->getScalarValue(mem1), memory->getScalarValue(mem2));
    } else if (isLeftAggregated && isRightAggregated) { // aggregated comparison
        AggregatedTypeIterator *it1 = getAggregatedIterator(memory, mem1), *it2 = getAggregatedIterator(memory, mem2);
        bool finalResult = true;
        while (it1->hasNext() && it2->hasNext()) {
            bool result;
            bool isLeftElementAggregated = memory->getFile()->isAggregatedType(it1->elementType()), isRightElementAggregated = memory->getFile()->isAggregatedType(it2->elementType());

            if(!isLeftElementAggregated && !isRightElementAggregated)
                result = aggregatedComparisonBetweenScalarValues(mode, **it1, **it2);
            else if (isLeftElementAggregated && isRightElementAggregated)
                result = aggregatedComparisonBetweenMemoryLocation(mode, **it1, **it2);
            else
                result = aggregatedComparisonBetweenScalarValueAndAggregated(mode,
                        memory->getScalarValue(
                                isLeftElementAggregated ? **it2 : **it1),
                                isLeftElementAggregated ? **it1 : **it2,
                                isRightElementAggregated);

            if(!result) {
                finalResult = false;
                break;
            }

            ++(*it1);
            ++(*it2);
        }

        finalResult = finalResult & ((mode != 5 && !it1->hasNext() && !it2->hasNext()) || (mode == 5));
        delete it1;
        delete it2;

        return  finalResult; // they
    } else { // scalar and aggregated comparison
        return aggregatedComparisonBetweenScalarValueAndAggregated(mode,
                memory->getScalarValue(
                        isLeftAggregated ? mem2 : mem1),
                        isLeftAggregated ? mem1 : mem2,
                        isRightAggregated);
    }
}

void InterpreterThread::op_cmp(unsigned flags, const uint16_t *params)
{
	// Params:
	// 0: Destination, memory location - store result here
	// 1: Source1, memory location
	// 2: Source2, memory location

    if(!memory->getFile()->isAggregatedType(params[0])) {
        bool result = aggregatedComparisonBetweenMemoryLocation(flags, params[1], params[2]);
        memory->setScalarValue(params[0], result);
    } else {
        std::cout << "op_cmp: non-aggregated comparison result not implemented" << std::endl;
    }
}

void InterpreterThread::op_tst(unsigned flags, const uint16_t *params)
{
	// Params:
	// 0: Destination, memory location - store result here
	// 1: Source, memory location

	if(!memory->getFile()->isAggregatedType(params[0])) {
        if(!memory->getFile()->isAggregatedType(params[1])) {
            int a = memory->getScalarValue(params[1]);
            memory->setScalarValue(params[0], aggregatedComparisonBetweenScalarValues(flags, a, 0));
        } else
            memory->setScalarValue(params[0], aggregatedComparisonBetweenScalarValueAndAggregated(flags, 0, params[1], false));
	} else {
        std::cout << "op_tst: non-aggregated comparison result not implemented" << std::endl;
    }
}

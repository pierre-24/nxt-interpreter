//
// Created by pierre on 10/04/2020.
//

#include "AggregatedTypeIterators.h"
#include "../System/VMMemory.h"

ArrayIterator::ArrayIterator(VMMemory* mem, unsigned entry): arrayDSTOCEntry(entry), i(0), memory(mem) {
    if(memory->getFile()->getTypeAtDSTOCIndex(arrayDSTOCEntry) != RXEFile::TC_ARRAY)
        throw std::runtime_error("that's not an array");
}

int32_t ArrayIterator::operator* () {
    return memory->getArrayElement(arrayDSTOCEntry, i);  // get array element at the current position
}

bool ArrayIterator::hasNext() {
    return i < memory->getArrayLength(arrayDSTOCEntry) - 1;
}

int ArrayIterator::elementType() {
    return memory->getFile()->getTypeAtDSTOCIndex(arrayDSTOCEntry + 1);
}

AggregatedTypeIterator* getAggregatedIterator(VMMemory*mem, unsigned memLoc) {
    if (mem->getFile()->getTypeAtDSTOCIndex(memLoc) == RXEFile::TC_ARRAY)
        return new ArrayIterator(mem, memLoc);
    else
        throw std::runtime_error("cluster iterator not implemented");
}
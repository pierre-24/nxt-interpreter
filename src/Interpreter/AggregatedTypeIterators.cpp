//
// Created by pierre on 10/04/2020.
//

#include <iostream>
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
    return i < memory->getArrayLength(arrayDSTOCEntry);
}

int ArrayIterator::elementType() {
    return memory->getFile()->getTypeAtDSTOCIndex(arrayDSTOCEntry + 1);
}

bool ArrayIterator::isAggregatedType() {
    return memory->getFile()->isAggregatedType(arrayDSTOCEntry + 1);
}

AggregatedTypeIterator* getAggregatedIterator(VMMemory*mem, unsigned memLoc) {
    if (mem->getFile()->getTypeAtDSTOCIndex(memLoc) == RXEFile::TC_ARRAY)
        return new ArrayIterator(mem, memLoc);
    if (mem->getFile()->getTypeAtDSTOCIndex(memLoc) == RXEFile::TC_CLUSTER)
        return new ClusterIterator(mem, memLoc);
    else
        throw std::runtime_error("unknown aggregate for iterator");
}

ClusterIterator::ClusterIterator(VMMemory* mem, unsigned entry): clusterDSTOCEntry(entry), i(0), real_i(1), memory(mem) {
    if(memory->getFile()->getTypeAtDSTOCIndex(clusterDSTOCEntry) != RXEFile::TC_CLUSTER)
        throw std::runtime_error("that's not a cluster");

    size = memory->getFile()->getDataDescriptorAtDSTOCIndex(entry);
}

AggregatedTypeIterator & ClusterIterator::operator++() {
    if(elementType() == RXEFile::TC_ARRAY) {
        real_i += 1; // skip array type
    } else if(elementType() == RXEFile::TC_CLUSTER) {
        real_i += int(memory->getFile()->getDataDescriptorAtDSTOCIndex(clusterDSTOCEntry + real_i)); // skip all cluster elements
    }

    i++;
    real_i++;

    return *this;
}

int32_t ClusterIterator::operator* () { // return the memory location if it is an aggregate, else return the corresponding scalar value
    if(memory->getFile()->isAggregatedType(clusterDSTOCEntry + real_i))
        return clusterDSTOCEntry + real_i;
    else
        return memory->getScalarValue(clusterDSTOCEntry + real_i);  // get array element at the current position
}

bool ClusterIterator::hasNext() {
    return i < size;
}

int ClusterIterator::elementType() {
    return memory->getFile()->getTypeAtDSTOCIndex(clusterDSTOCEntry + real_i);
}

bool ClusterIterator::isAggregatedType() {
    return memory->getFile()->isAggregatedType(clusterDSTOCEntry + real_i);
}

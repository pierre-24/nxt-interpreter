//
// Created by pierre on 07/04/2020.
//

#include <string>
#include <cstring>
#include <iostream>
#include "VFile.h"

VFile::VFile(unsigned size): currentSize(size), nextWritePosition(0) {
    content = new char[size];
}

VFile::~VFile() {
    delete content;
}

unsigned VFile::read(unsigned begin, unsigned length, char *data, unsigned &readLength) const {
    if (begin > currentSize)
        return VFileError::GenericError;

    if (begin + length > currentSize)
        readLength = currentSize - begin;
    else
        readLength = length;

    memcpy(data, content+begin, readLength);

    return (readLength == length) ? VFileError::Success : VFileError::_EOF;
}

unsigned VFile::write(unsigned position, const char *data, unsigned length, unsigned& writeLength) {
    if(position > currentSize)
        return VFileError::GenericError;

    if (position + length > currentSize)
        writeLength = currentSize - position;
    else
        writeLength = length;

    memcpy(content+position, data, writeLength);

    if ((position + writeLength) > nextWritePosition)
        nextWritePosition = position + writeLength;

    return (writeLength != length) ? VFileError::PartialWrite : VFileError::Success;
}



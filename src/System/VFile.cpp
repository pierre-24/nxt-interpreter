//
// Created by pierre on 07/04/2020.
//

#include <string>
#include <cstring>
#include "VFile.h"

VFile::VFile(unsigned size): currentSize(size), nextWritePosition(0) {
    content = new char[size];
}

VFile::~VFile() {
    delete content;
}

unsigned VFile::read(unsigned begin, unsigned length, char **data, unsigned &readLength) const {
    readLength = 0;
    if (begin > currentSize)
        return VFileError::GenericError;

    if (begin + length > currentSize)
        readLength = currentSize - begin;
    else
        readLength = length;

    *data = new char[readLength];
    memcpy(*data, content+begin, readLength);

    return VFileError::Success;
}

unsigned VFile::write(unsigned position, const char *data, unsigned length, unsigned& writeLength) {
    if(position > currentSize)
        return VFileError::GenericError;

    if (position + length > currentSize)
        writeLength = currentSize - position;

    memcpy(content+position, data, writeLength);

    if (position + writeLength > nextWritePosition)
        nextWritePosition = position + writeLength;

    return (writeLength != length) ? VFileError::PartialWrite : VFileError::Success;
}



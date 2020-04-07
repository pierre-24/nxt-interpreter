//
// Created by pierre on 07/04/2020.
//

#include <string>
#include <cstring>
#include "VFile.h"

VFile::VFile(): currentSize(0) {
    content = new char[VFileConstant::Size];
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
    if (currentSize == VFileConstant::Size)
        return VFileError::FileFull;

    if(position > currentSize)
        position = currentSize;

    if (position + length > VFileConstant::Size)
        writeLength = VFileConstant::Size - position;

    memcpy(content+position, data, writeLength);
    currentSize += length;

    return (writeLength != length) ? VFileError::PartialWrite : VFileError::Success;
}

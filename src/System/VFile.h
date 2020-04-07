//
// Created by pierre on 07/04/2020.
//

/*!
 * @abstract represent a file
 */

namespace VFileConstant {
    const unsigned Size = 1024; // bytes
}

namespace VFileError {
    const unsigned Success = 0x0000;
    const unsigned PartialWrite = 0x8400;
    const unsigned GenericError = 0x8a00;
    const unsigned FileFull = 0x8a00;
}

class VFile {
    char* content;
    unsigned currentSize;

public:
    VFile();
    ~VFile();

    unsigned read(unsigned begin, unsigned length, char **data, unsigned &readLength) const;
    unsigned write(unsigned position, const char* data, unsigned length, unsigned& writeLength);
};

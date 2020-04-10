//
// Created by pierre on 07/04/2020.
//

#include <map>
#include <string>

class VFile;

namespace VFileError {
    const unsigned FileNotFound = 0x8700;
    const unsigned FileBusy = 0x8b00;
    const unsigned FileFull = 0x8e00;
    const unsigned FileExists = 0x8f00;
    const unsigned IllegalFileName = 0x9200;
    const unsigned IllegalHandle = 0x9300;
}

namespace VFileSystemConstant {
    const unsigned maxFile = 15; // there is a limit of 16 opened file in the firmware
}

struct FileHandle;

/*!
 * @abstract Create a virtual file system
 */
class VFileSystem {

    std::map<std::string, VFile*> files;
    std::map<std::string, unsigned> openedHandles;

    FileHandle* fileHandles[VFileSystemConstant::maxFile];

    unsigned createFile(const std::string &name, unsigned size);
    unsigned renameFile(const std::string& previous, const std::string& current);
    unsigned deleteFile(const std::string& name);
    bool fileExists(const std::string& name);

    VFile* getFile(const std::string& name) noexcept(false);
    FileHandle * getFileHandle(unsigned handle) noexcept(false);

    unsigned openFile(const std::string &name, unsigned mode, unsigned &handle);
    unsigned closeFile(unsigned handle);

public:
    VFileSystem();
    ~VFileSystem();

    void FileOpenRead(unsigned& status, unsigned& handle, const char* name, unsigned length);
    void FileOpenWrite(unsigned& status, unsigned& handle, const char* name, unsigned length);
    void FileOpenAppend(unsigned& status, unsigned& handle, const char* name, unsigned length);

    void FileRead(unsigned& status, unsigned handle, char* buff, unsigned &length);
    void FileWrite(unsigned& status, unsigned handle, const char* buff, unsigned &length);

    void FileResolveHandle(unsigned& status, unsigned& handle, bool& write, const char* filename);

    // TODO: allow handler 0 (= standard output) only for write !

    void FileClose(unsigned& status, unsigned handle);
};

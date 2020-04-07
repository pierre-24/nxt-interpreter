//
// Created by pierre on 07/04/2020.
//

#include <map>
#include <string>

class VFile;

namespace VFileError {
    const unsigned FileNotFound = 0x8700;
    const unsigned FileExists = 0x8f00;
}

/*!
 * @abstract Create a virtual file system
 */
class VFileSystem {

    std::map<std::string, VFile*> files;

public:
    VFileSystem() = default;
    ~VFileSystem();

    unsigned createFile(const std::string& name);
    unsigned renameFile(const std::string& previous, const std::string& current);
    unsigned deleteFile(const std::string& name);
    bool fileExists(const std::string& name);

    VFile* getFile(const std::string& name) noexcept(false);
};

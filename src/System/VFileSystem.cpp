//
// Created by pierre on 07/04/2020.
//

#include "VFileSystem.h"
#include "VFile.h"


VFileSystem::~VFileSystem() {
    for (std::pair<std::string, VFile*> p: files)
        delete p.second;
}


unsigned VFileSystem::createFile(const std::string &name) {
    if(fileExists(name))
        return VFileError::FileExists;

    files.insert({std::string(name), new VFile()});
    return VFileError::Success;
}

unsigned VFileSystem::renameFile(const std::string &previous, const std::string &current) {
    if(fileExists(current))
        return VFileError::FileExists;

    auto it = files.find(previous);
    if (it == files.end())
        return VFileError::FileNotFound;

    files.insert({std::string(current), (*it).second});
    files.erase(it);

    return VFileError::Success;
}

unsigned VFileSystem::deleteFile(const std::string &name) {
    auto it = files.find(name);
    if (it == files.end())
        return VFileError::FileNotFound;

    delete (*it).second;
    files.erase(it);

    return VFileError::Success;
}

bool VFileSystem::fileExists(const std::string &name) {
    return files.count(name) > 0;
}

VFile *VFileSystem::getFile(const std::string &name) noexcept(false) {
    auto it = files.find(name);
    if (it == files.end())
        throw std::runtime_error("file not found");
    else
        return (*it).second;
}

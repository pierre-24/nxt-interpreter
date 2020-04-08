//
// Created by pierre on 07/04/2020.
//

#include "VFileSystem.h"
#include "VFile.h"

#include <iostream>

struct FileHandler {
    VFile* file;
    unsigned flags;
    unsigned cursor;
};


namespace FileHandlerConstants {
    const unsigned OpenRead = 0x01;
    const unsigned OpenWrite = 0x02;
    const unsigned OpenAppend = 0x04;
}


VFileSystem::VFileSystem() {
    for(auto & fileHandler : fileHandlers)
        fileHandler = nullptr;
}

VFileSystem::~VFileSystem() {
    for (auto& p: files)
        delete p.second;

    for(auto & fileHandler : fileHandlers)
        delete fileHandler;
}


unsigned VFileSystem::createFile(const std::string &name, unsigned size) {
    if(fileExists(name))
        return VFileError::FileExists;

    files.insert({std::string(name), new VFile(size)});
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

unsigned VFileSystem::openFile(const std::string &name, unsigned mode, unsigned &handle) {
    handle = VFileSystemConstant::maxFile;
    VFile *f = nullptr;

    try {
        f = getFile(name);
    } catch (std::runtime_error& e) {
        return VFileError::FileNotFound;
    }

    if(openedHandles.count(name) > 0)
        return VFileError::FileBusy;

    for(int i=0; i < VFileSystemConstant::maxFile; i++) {
        if(fileHandlers[i] == nullptr) { // get the next available handler
            unsigned cursor = 0;

            if (mode & FileHandlerConstants::OpenAppend) {
                if(f->isFull())
                    return VFileError::FileFull;

                cursor = f->getNextWritePosition();
            }

            auto* fileHandler = new FileHandler;
            fileHandler->file = f;
            fileHandler->flags = mode;
            fileHandler->cursor = cursor;
            fileHandlers[i] = fileHandler;
            handle = i + 1;
            openedHandles.insert({std::string(name), handle});
            break;
        }
    }

    return VFileError::Success;
}

unsigned VFileSystem::closeFile(unsigned handle) {
    handle -= 1;
    if (handle >= VFileSystemConstant::maxFile)
        return VFileError::IllegalHandle;

    if(fileHandlers[handle] == nullptr)
        return VFileError::IllegalHandle;

    delete(fileHandlers[handle]);
    fileHandlers[handle] = nullptr;

    return VFileError::Success;
}

void VFileSystem::FileOpenWrite(unsigned &status, unsigned &handle, const char *name, unsigned length) {
    status = createFile(name, length);
    if (status == VFileError::Success)
        status = openFile(name, FileHandlerConstants::OpenWrite, handle);
}

void VFileSystem::FileOpenRead(unsigned &status, unsigned &handle, const char *name, unsigned length) {
    status = createFile(name, length);
    if (status == VFileError::Success)
        status = openFile(name, FileHandlerConstants::OpenRead, handle);
}

void VFileSystem::FileOpenAppend(unsigned &status, unsigned &handle, const char *name, unsigned length) {
    status = createFile(name, length);
    if (status == VFileError::Success)
        status = openFile(name, FileHandlerConstants::OpenWrite | FileHandlerConstants::OpenAppend, handle);
}

void VFileSystem::FileClose(unsigned &status, unsigned handle) {
    status = closeFile(handle);
}

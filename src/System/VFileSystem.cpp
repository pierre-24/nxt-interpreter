//
// Created by pierre on 07/04/2020.
//

#include <iostream>
#include "VFileSystem.h"
#include "VFile.h"

struct FileHandle {
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
    for(auto & fileHandle : fileHandles)
        fileHandle = nullptr;
}

VFileSystem::~VFileSystem() {
    for (auto& p: files)
        delete p.second;

    for(auto & fileHandler : fileHandles)
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

FileHandle * VFileSystem::getFileHandle(unsigned handle) noexcept(false) {
    if (handle > VFileSystemConstant::maxFile)
        throw std::runtime_error("file not found");

    if(fileHandles[handle - 1] == nullptr)
        throw std::runtime_error("file not found");
    else
        return fileHandles[handle - 1];
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
        if(fileHandles[i] == nullptr) { // get the next available handler
            unsigned cursor = 0;

            if (mode & FileHandlerConstants::OpenAppend) {
                if(f->isFull())
                    return VFileError::FileFull;

                cursor = f->getNextWritePosition();
            }

            auto* fileHandle = new FileHandle;
            fileHandle->file = f;
            fileHandle->flags = mode;
            fileHandle->cursor = cursor;
            fileHandles[i] = fileHandle;
            handle = i + 1;
            openedHandles.insert({std::string(name), handle});
            break;
        }
    }

    return VFileError::Success;
}

unsigned VFileSystem::closeFile(unsigned handle) {
    try {
        auto* fhandle = getFileHandle(handle);
        delete(fhandle);
    } catch (std::runtime_error& e) {
        return VFileError::IllegalHandle;
    }

    fileHandles[handle - 1] = nullptr;

    auto it = openedHandles.begin();

    for(;it != openedHandles.end(); it++) {
        if ((*it).second == handle) {
            openedHandles.erase(it);
            break;
        }
    }

    return VFileError::Success;
}

void VFileSystem::FileOpenWrite(unsigned &status, unsigned &handle, const char *name, unsigned length) {
    status = createFile(name, length);
    if (status == VFileError::Success)
        status = openFile(name, FileHandlerConstants::OpenWrite, handle);
}

void VFileSystem::FileOpenRead(unsigned &status, unsigned &handle, const char *name, unsigned length) {
    status = openFile(name, FileHandlerConstants::OpenRead, handle);
}

void VFileSystem::FileOpenAppend(unsigned &status, unsigned &handle, const char *name, unsigned length) {
    status = openFile(name, FileHandlerConstants::OpenWrite | FileHandlerConstants::OpenAppend, handle);
}

void VFileSystem::FileClose(unsigned &status, unsigned handle) {
    status = closeFile(handle);
}

void VFileSystem::FileRead(unsigned &status, unsigned handle, char *buff, unsigned &length) {
    try {
        auto fhandle = getFileHandle(handle);
        status = fhandle->file->read(fhandle->cursor, length, buff, length);
        fhandle->cursor += length;
    } catch (std::runtime_error & e) {
        status = VFileError::IllegalHandle;
    }
}

void VFileSystem::FileWrite(unsigned &status, unsigned handle, const char *buff, unsigned &length) {
    try {
        auto fhandle = getFileHandle(handle);
        status = fhandle->file->write(fhandle->cursor, buff, length, length);
        fhandle->cursor += length;
    } catch (std::runtime_error & e) {
        status = VFileError::IllegalHandle;
    }
}

void VFileSystem::FileResolveHandle(unsigned &status, unsigned &handle, bool &write, const char *filename) {
    auto it = openedHandles.find(std::string(filename));
    if(it == openedHandles.end()) {
        status = VFileError::FileNotFound;
    } else {
        handle = it->second;
        write = getFileHandle(handle)->flags & FileHandlerConstants::OpenWrite;
        status = VFileError::Success;
    }
}

void VFileSystem::FileRename(unsigned &status, const char *prev, const char *curr) {
    if(openedHandles.count(std::string(prev)) > 0)
        status = VFileError::FileBusy;
    else
        status = renameFile(std::string(prev), std::string(curr));
}

void VFileSystem::FileDelete(unsigned &status, const char *filename) {
    if(openedHandles.count(std::string(filename)) > 0)
        status = VFileError::FileBusy;
    else
        status = deleteFile(std::string(filename));
}

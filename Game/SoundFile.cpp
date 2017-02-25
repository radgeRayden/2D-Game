#include "SoundFile.h"

#include <memory>
#include <string>

#include <cpplog/cpplog.hpp>

static cpplog::StdErrLogger logger;

SoundFile::SoundFile(const std::string& filename) {
    if (!PHYSFS_exists(filename.c_str())) {
        LOG_ERROR(logger) << "Could not open " << filename << "." << std::endl;
        loadedSuccessfully = false;
        return;
    }
    fileHandle = PHYSFS_openRead(filename.c_str());
    loadedSuccessfully = true;
}


SoundFile::~SoundFile() {
    if (loadedSuccessfully) {
        PHYSFS_close(fileHandle);
    }
}

int SoundFile::eof() {
    return PHYSFS_eof(fileHandle);
}
unsigned int SoundFile::read(unsigned char *aDst, unsigned int aBytes) {
    auto objRead = PHYSFS_read(fileHandle, aDst, aBytes, 1);
    return aBytes * objRead;
}

unsigned int SoundFile::length() {
    return PHYSFS_fileLength(fileHandle);
}

void SoundFile::seek(int aOffset) {
    if (aOffset >= 0) {
        PHYSFS_seek(fileHandle, aOffset);
    }
    else {
        PHYSFS_seek(fileHandle, length() + aOffset);
    }
}

unsigned int SoundFile::pos() {
    return PHYSFS_tell(fileHandle);
}

bool SoundFile::IsValid() {
    return loadedSuccessfully;
}
#include "SoundFile.h"

#include <memory>
#include <string>

#include <PhysFS++/physfs.hpp>

SoundFile::SoundFile(const std::string& filename) {
    inputStream = std::make_unique<PhysFS::ifstream>(filename);
}


SoundFile::~SoundFile() {
}

int SoundFile::eof() {
    return inputStream->eof();
}
unsigned int SoundFile::read(unsigned char *aDst, unsigned int aBytes) {
    inputStream->read((char *)aDst, aBytes);
    return aBytes;
}

unsigned int SoundFile::length() {
    return inputStream->length();
}

void SoundFile::seek(int aOffset) {
    if (aOffset >= 0) {
        inputStream->seekg(aOffset);
    }
    else {
        inputStream->seekg(length() + aOffset);
    }
}

unsigned int SoundFile::pos() {
    return inputStream->tellg();
}
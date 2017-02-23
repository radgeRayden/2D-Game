#include "SoundFile.h"

SoundFile::SoundFile(const char* filename) {
    inputStream = new PhysFS::ifstream(filename);
}


SoundFile::~SoundFile() {
    delete inputStream;
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
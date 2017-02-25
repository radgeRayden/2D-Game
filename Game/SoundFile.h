#pragma once
#include <string>
#include <memory>

#include <soloud_file.h>
#include <physfs.h>

class SoundFile : public SoLoud::File 
{
public:
    SoundFile(const std::string& fileName);
    ~SoundFile();
    int eof();
    unsigned int read(unsigned char *aDst, unsigned int aBytes);
    unsigned int length();
    void seek(int aOffset);
    unsigned int pos();
    bool IsValid();
private:
    PHYSFS_File* fileHandle;
    bool loadedSuccessfully = false;
};


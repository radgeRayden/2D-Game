#pragma once
#include <soloud_file.h>
#include <PhysFS++/physfs.hpp>
class SoundFile : public SoLoud::File 
{
public:
    SoundFile(const char *fileName);
    ~SoundFile();
    virtual int eof();
    virtual unsigned int read(unsigned char *aDst, unsigned int aBytes);
    virtual unsigned int length();
    void seek(int aOffset);
    virtual unsigned int pos();
private:
    PhysFS::ifstream *inputStream;
};


#pragma once

#include <cstdint>

class Timer {
public:
    Timer();
    float Step();
    void Pause();
    void Start();
    void Reset();
    uint32_t Tell();
    void SetScale(float scale);
    bool Paused();
    ~Timer();
private:
    float timeScale = 1.0f;
    uint32_t compressedTime = 0;
    uint32_t referenceTime;
    uint64_t lastStepMeasure = 0;
    bool isPaused;
};


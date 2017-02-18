#pragma once

#include <cstdint>

/*
Performs time measurement, absolute or scaled. Results are in miliseconds unless otherwise noted.
*/
class Timer {
public:
    Timer();
    /*
    Gets the difference between last step time and now (starting at creation), in seconds.
    Useful for scaled delta-time measurement.
    */
    float Step();
    /*
    Stores time elapsed since creation (scaled). Does not increment it until started again.
    Stepping is scaled by zero if paused.
    */
    void Pause();
    /*
    Resumes after a pause.
    */
    void Start();
    /*
    Discards all measurements and restarts the timer.
    */
    void Reset();
    /*
    Gets time elapsed after creation (scaled).
    For real time after creation, never change time scale.
    */
    uint32_t Tell();
    /*
    Sets time measurement scale. Stepping and telling will retrieve scaled timespans.
    Do note that changing time scale will not scale the whole timeline, only affecting measurements after the fact.
    */
    void SetScale(float scale);
    /*
    Returns true if the timer is paused.
    */
    bool Paused();
    ~Timer();
private:
    float timeScale = 1.0f;
    uint32_t compressedTime = 0;
    uint32_t referenceTime;
    uint64_t lastStepMeasure = 0;
    bool isPaused;
};
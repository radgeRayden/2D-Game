#include "Timer.h"
#include <cstdint>
#include <SDL2/SDL.h>

Timer::Timer() {
    referenceTime = SDL_GetTicks();
    Step(); //initializes lastMeasure
}

float Timer::Step() {
    uint64_t currentMeasure = SDL_GetPerformanceCounter();
    uint64_t delta = currentMeasure - lastStepMeasure;
    lastStepMeasure = currentMeasure;

    return timeScale * (delta / static_cast<float>(SDL_GetPerformanceFrequency()));
}

void Timer::Pause() {
    compressedTime = Tell();
    isPaused = true;
}

void Timer::Start() {
    referenceTime = SDL_GetTicks();
    isPaused = false;
}

void Timer::Reset() {
    compressedTime = 0;
    referenceTime = SDL_GetTicks();
    Step();
}

uint32_t Timer::Tell() {
    //TODO: return scaled time since creation
    //does this work?
    return compressedTime + (SDL_GetTicks() - referenceTime) * timeScale;
}

void Timer::SetScale(float scale) {
    timeScale = scale;
    //TODO: set compressedTime taking scaled time into account
}

bool Timer::Paused() {
    return isPaused;
}

Timer::~Timer() {
}

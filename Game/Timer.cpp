#include "Timer.h"
#include <cstdint>
#include <SDL2/SDL.h>

Timer::Timer() {
    referenceTime = SDL_GetTicks();
    Step(); //initializes lastMeasure
}

float Timer::Step() {
    //best resolution guaranteed
    uint64_t currentMeasure = SDL_GetPerformanceCounter();
    uint64_t delta = currentMeasure - lastStepMeasure;
    lastStepMeasure = currentMeasure;

    if (!isPaused) {
        return timeScale * (delta / static_cast<float>(SDL_GetPerformanceFrequency()));
    }
    else {
        return 0.0f;
    }
}

void Timer::Pause() {
    compressedTime = Tell();
    isPaused = true;
}

void Timer::Start() {
    if (isPaused) {
        referenceTime = SDL_GetTicks();
        isPaused = false;
    }
}

void Timer::Reset() {
    compressedTime = 0;
    referenceTime = SDL_GetTicks();
    Step();
}

uint32_t Timer::Tell() {
    //compressed time takes into account all scaling and rescaling done, without storing the operations.
    return compressedTime + (SDL_GetTicks() - referenceTime) * timeScale;
}

void Timer::SetScale(float scale) {
    //compress time yet again
    compressedTime = Tell();
    //reset the reference from which Tell() measurements are taken, making compressedTime effective
    referenceTime = SDL_GetTicks();
    timeScale = scale;
}

bool Timer::Paused() {
    return isPaused;
}

Timer::~Timer() {
}

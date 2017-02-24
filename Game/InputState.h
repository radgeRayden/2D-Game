#pragma once
#include <unordered_map>

#include <SDL2/SDL.h>

class InputState {
public:
    InputState();
    void SetKey(SDL_Keycode keyCode, bool value);
    bool GetKey(SDL_Keycode keyCode) const;
    void Clear();
    ~InputState();
private:
    std::unordered_map<SDL_Keycode, bool> keys {
        { SDLK_w, false },
        { SDLK_a, false },
        { SDLK_s, false },
        { SDLK_d, false },
        { SDLK_UP, false },
        { SDLK_LEFT, false },
        { SDLK_DOWN, false },
        { SDLK_RIGHT, false },
        { SDLK_SPACE, false },
        { SDLK_j, false }
    };
};


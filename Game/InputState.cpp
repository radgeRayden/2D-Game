#include "InputState.h"

InputState::InputState() {
}

void InputState::SetKey(SDL_Keycode keyCode, bool value) {
    keys[keyCode] = value;
}

bool InputState::GetKey(SDL_Keycode keyCode) const {
    return keys.at(keyCode);
}

void InputState::Clear() {
    for (auto& key : keys) {
        key.second = false;
    }
}

InputState::~InputState() {
}

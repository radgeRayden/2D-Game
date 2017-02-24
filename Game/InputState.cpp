#include "InputState.h"

InputState::InputState() {
}

void InputState::SetKey(SDL_Keycode keyCode, bool value) {
    keys[keyCode] = value;
}

bool InputState::GetKey(SDL_Keycode keyCode) const {
    auto keyValue = keys.find(keyCode);
    if (keyValue != keys.end()) {
        return keyValue->second;
    }
    return false;
}

void InputState::Clear() {
    for (auto& key : keys) {
        key.second = false;
    }
}

InputState::~InputState() {
}

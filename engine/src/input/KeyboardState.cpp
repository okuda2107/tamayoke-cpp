#include "input/KeyboardState.h"

bool KeyboardState::GetKeyValue(SDL_Scancode keyCode) const {
    return mCurrState[keyCode];
}

ButtonState KeyboardState::GetKeyState(SDL_Scancode keyCode) const {
    if (!mPrevState[keyCode]) {
        if (!mCurrState[keyCode])
            return ENone;
        else
            return EPressed;
    } else {
        if (!mCurrState[keyCode])
            return EReleased;
        else
            return EHeld;
    }
}

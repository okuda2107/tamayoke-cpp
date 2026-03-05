#include "input/MouseState.h"

bool MouseState::GetButtonValue(int button) const {
    return (mCurrButtons & SDL_BUTTON(button));
}

ButtonState MouseState::GetButtonState(int button) const {
    if (!(mPrevButtons & SDL_BUTTON(button))) {
        if (!(mCurrButtons & SDL_BUTTON(button)))
            return ENone;
        else
            return EPressed;
    } else {
        if (!(mCurrButtons & SDL_BUTTON(button)))
            return EReleased;
        else
            return EHeld;
    }
}

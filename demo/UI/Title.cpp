#include "Title.h"

#include "SDL.h"
#include "game/StateManager.h"
#include "input/InputState.h"

Title::Title(UISystem* system, BonfireUIDeps deps) : BonfireUI(system, deps) {
    mStateManager.mState = GameState::EPaused;
    SetTitle("Chillut's Fire", Color::White, 72);
    mTitlePos = Vector2(0.0f, 300.0f);
}

Title::~Title() { mStateManager.mState = GameState::EGameplay; }

void Title::ProcessInput(const InputState& state) {
    auto iter = state.EventMap.find(SDL_KEYDOWN);
    if (iter == state.EventMap.end()) return;

    Close();
}

#include "GameOverUI.h"

#include <sstream>

#include "../actor/Bonfire.h"
#include "SDL.h"
#include "game/StateManager.h"
#include "input/InputState.h"

GameOverUI::GameOverUI(UISystem* system, BonfireUIDeps deps)
    : BonfireUI(system, deps) {
    deps.stateManager.mState = GameState::EPaused;
}

void GameOverUI::SetTime(int time) {
    std::stringstream ss;
    ss << "Time: " << time;
    SetTitle(ss.str(), Color::White, 72);
}

void GameOverUI::ProcessInput(const InputState& state) {
    auto iter = state.EventMap.find(SDL_KEYDOWN);
    if (iter == state.EventMap.end()) return;

    Close();
}

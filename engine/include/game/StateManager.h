#pragma once
#include "game/base/GameSystemBase.h"

enum class GameState {
    EGameplay,
    EPaused,
};

// Gameのループ状態を制御するためのクラス
class StateManager : public GameSystemBase {
   public:
    StateManager() : mState(GameState::EGameplay) {}
    GameState mState;
};

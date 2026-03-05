#pragma once
#include <unordered_map>

#include "KeyboardState.h"
#include "MouseState.h"
#include "base/InputStateBase.h"

// 入力機器から取得した情報をまとめた構造体
struct InputState : InputStateBase {
    // 発生している入力イベントをマップで持っておく
    std::unordered_map<Uint32, SDL_Event> EventMap;

    KeyboardState Keyboard;
    MouseState Mouse;
};

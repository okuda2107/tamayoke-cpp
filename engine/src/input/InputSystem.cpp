#include "input/InputSystem.h"

#include <unordered_map>

#include "input/InputState.h"

InputSystem::~InputSystem() { Shutdown(); }

bool InputSystem::Initialize() {
    // キーボード
    mState.Keyboard = KeyboardState();
    // 現在の状態を示すポインタを保存
    mState.Keyboard.mCurrState = SDL_GetKeyboardState(NULL);
    // 前回の状態を示すメモリをクリア
    memset(mState.Keyboard.mPrevState, 0, SDL_NUM_SCANCODES);

    // マウス
    mState.Mouse = MouseState();
    mState.Mouse.mCurrButtons = 0;
    mState.Mouse.mPrevButtons = 0;
    mState.Mouse.mIsRelative = false;

    // イベントマップ
    mState.EventMap = std::unordered_map<Uint32, SDL_Event>();

    return true;
}

void InputSystem::Shutdown() { mState.EventMap.clear(); }

void InputSystem::Update() {
    // 前処理
    // キーボード
    memcpy(mState.Keyboard.mPrevState, mState.Keyboard.mCurrState,
           SDL_NUM_SCANCODES);

    // マウス
    mState.Mouse.mPrevButtons = mState.Mouse.mCurrButtons;
    mState.Mouse.mScrollWheel = Vector2::Zero;

    // 更新処理
    // イベントの取得 と 入力機器の状態を取得
    mState.EventMap.clear();
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_MOUSEWHEEL:
                mState.Mouse.mScrollWheel =
                    Vector2(static_cast<float>(event.wheel.x),
                            static_cast<float>(event.wheel.y));
                break;

            default:
                mState.EventMap.emplace(event.type, event);
        }
    }

    // マウス
    int x = 0, y = 0;
    if (mState.Mouse.mIsRelative) {
        mState.Mouse.mCurrButtons = SDL_GetRelativeMouseState(&x, &y);
    } else {
        mState.Mouse.mCurrButtons = SDL_GetMouseState(&x, &y);
    }
    mState.Mouse.mMousePos.x = static_cast<float>(x);
    mState.Mouse.mMousePos.y = static_cast<float>(y);
}

void InputSystem::ProcessMetrics(const InputSystemMetrics& metrics) {
    if (mState.Mouse.mIsRelative != metrics.mRelativeMouseMode)
        SetRelativeMouseMode(metrics.mRelativeMouseMode);
}

void InputSystem::SetRelativeMouseMode(bool value) {
    SDL_bool set = value ? SDL_TRUE : SDL_FALSE;
    SDL_SetRelativeMouseMode(set);
    mState.Mouse.mIsRelative = value;
}

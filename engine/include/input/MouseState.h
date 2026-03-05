#pragma once
#include "ButtonState.h"
#include "SDL.h"
#include "core/Math.h"

class MouseState {
    friend class InputSystem;

    // マウスの位置を格納
    Vector2 mMousePos;
    // スクロールの状態を格納
    Vector2 mScrollWheel;
    // ボタンのデータを格納
    Uint32 mCurrButtons;
    Uint32 mPrevButtons;

    // 相対マウスモード
    bool mIsRelative;

   public:
    // マウスポジション用
    // +x: 右 +y: 下 SDLの2次元座標
    const Vector2& GetPostion() const { return mMousePos; }

    // マウスボタン用
    bool GetButtonValue(int button) const;
    ButtonState GetButtonState(int button) const;
};

#pragma once
#include "InputState.h"
#include "SDL.h"
#include "base/InputSystemBase.h"
#include "runtime/RuntimeData.h"

// 入力の状態を管理
class InputSystem : public InputSystemBase<InputState, InputSystemMetrics> {
    void Shutdown();

   public:
    ~InputSystem() override;

    bool Initialize();

    // 入力状態更新処理
    void Update();

    void ProcessMetrics(const InputSystemMetrics& metrics);

    // 相対マウスモードを有効 / 無効
    void SetRelativeMouseMode(bool value);
};

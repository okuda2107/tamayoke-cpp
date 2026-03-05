#pragma once
#include <type_traits>

#include "IInputSystem.h"

// 入力の状態を管理
/*
    InputState: 入力情報を格納
*/
template <typename InputState, typename InputSystemMetrics>
class InputSystemBase : public IInputSystem {
    static_assert(std::is_base_of<InputStateBase, InputState>::value,
                  "InputState must derive from InputStateBase");

    static_assert(
        std::is_base_of<InputSystemMetricsBase, InputSystemMetrics>::value,
        "InputSystemMetrics must derive from InputSystemMetricsBase");

   protected:
    InputState mState;

   public:
    InputSystemBase() = default;
    virtual ~InputSystemBase() = default;

    // 更新処理
    // 入力状態を定期的に問い合わせる (ポーリング)
    virtual void Update() = 0;

    virtual void ProcessMetrics(const InputSystemMetrics& metrics) = 0;

    // Engineで使われるためのエントリポイント
    void IUpdate() final { Update(); }

    const struct InputStateBase& IGetState() const final { return mState; }

    void IProcessMetrics(const struct InputSystemMetricsBase& metrics) final {
        ProcessMetrics(static_cast<const InputSystemMetrics&>(metrics));
    }
};

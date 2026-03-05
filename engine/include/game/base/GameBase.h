#pragma once
#include <type_traits>

#include "IGame.h"

// ゲームの内部での処理定義を責務とする
template <typename InputState, typename RenderData, typename GameData,
          typename GameMetrics>
class GameBase : public IGame {
    static_assert(std::is_base_of<InputStateBase, InputState>::value,
                  "InputState must derive from InputStateBase");

    static_assert(std::is_base_of<RenderDataBase, RenderData>::value,
                  "RenderData must derive from RenderDataBase");

    static_assert(std::is_base_of<GameDataBase, GameData>::value,
                  "GameData must derive from GameDataBase");

    static_assert(std::is_base_of<GameMetricsBase, GameMetrics>::value,
                  "GameMetrics must derive from GameMetricsBase");

   public:
    GameBase() = default;
    virtual ~GameBase() = default;

    // 入力を処理
    virtual void ProcessInput(const InputState& state) = 0;
    // deltatimeに基づいてgameを更新する
    // GameDataは更新結果を外部に伝達したいときのコンテナ
    virtual const GameData& Update(float deltatime,
                                   const GameMetrics& metrics) = 0;
    // gameの出力を生成
    virtual const RenderData& GenerateRenderData() = 0;

    // Engineで使われるためのエントリポイント
    void IProcessInput(const struct InputStateBase& state) final {
        ProcessInput(static_cast<const InputState&>(state));
    }
    const struct GameDataBase& IUpdate(
        float deltatime, const struct GameMetricsBase& metrics) final {
        return Update(deltatime, static_cast<const GameMetrics&>(metrics));
    }
    const struct RenderDataBase& IGenerateRenderData() final {
        return GenerateRenderData();
    }
};

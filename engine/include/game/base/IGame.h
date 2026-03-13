#pragma once
#include <type_traits>

#include "GameSystemVector.h"

// ゲームの内部での処理定義を責務とする
class IGame {
   public:
    class InputProcessorBase* input;
    class UpdaterBase* update;
    class RenderDataGeneratorBase* render;

    // ゲームで用いられるサブシステムを抽象化して持っておく
    GameSystemVector mSystems;

    IGame() = default;
    virtual ~IGame() = default;

    // Engineで使われるためのエントリポイント
    void IProcessInput(const struct InputStateBase& state);
    const struct GameDataBase& IUpdate(float deltatime,
                                       const struct GameMetricsBase& metrics);
    const struct RenderDataBase& IGenerateRenderData();
};

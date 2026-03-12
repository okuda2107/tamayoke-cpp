#pragma once
#include "game/base/GameImplBase.h"
#include "input/pose/InputState.h"
#include "renderer/RenderData.h"

class TamayokeGame : public GameImplBase<pose::InputState, RenderData,
                                         GameFrameResult, GameMetricsBase> {
    GameFrameResult mFrameResult;

   public:
    TamayokeGame();
    virtual ~TamayokeGame() = default;

    bool Initialize();

    void ProcessInput(const pose::InputState& state) override;
    const struct GameFrameResult& Update(
        float deltatime, const struct GameMetricsBase& metrics) override;
    const struct RenderData& GenerateRenderData() override;
};

#pragma once
#include "game/base/GameBase.h"
#include "game/base/InputProcessor.h"
#include "game/base/RenderDataGenerator.h"
#include "game/base/Updater.h"
#include "input/pose/InputState.h"
#include "renderer/RenderData.h"
#include "runtime/RuntimeData.h"

class TamayokeGame : public GameBase,
                     public InputProcessor<pose::InputState>,
                     public Updater<GameFrameResult, GameMetricsBase>,
                     public RenderDataGenerator<RenderData> {
   protected:
    GameFrameResult mFrameResult;

   public:
    TamayokeGame();
    virtual ~TamayokeGame() = default;

    bool Initialize();

    void ProcessInput(const pose::InputState& state) override;
    const struct GameFrameResult& Update(
        float deltatime, const struct GameMetricsBase& metrics) override;
    const struct RenderData& GenerateRenderData() override;

    // AudioSystemにbankデータを流し込む
    void LoadAudioBank(const std::string& name);
};

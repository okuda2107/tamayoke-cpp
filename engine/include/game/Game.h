#pragma once
#include <type_traits>

#include "base/GameImplBase.h"
#include "input/InputState.h"
#include "renderer/RenderData.h"
#include "runtime/RuntimeData.h"

class Game : public GameImplBase<InputState, RenderData, GameFrameResult,
                                 GameMetricsBase> {
    GameFrameResult mFrameResult;

   public:
    Game();
    virtual ~Game() = default;

    bool Initialize();

    void ProcessInput(const InputState& state) override;
    const struct GameFrameResult& Update(
        float deltatime, const struct GameMetricsBase& metrics) override;
    const struct RenderData& GenerateRenderData() override;

    // RenderDBに環境光のデータを流し込む
    void SetAmbientLight(Vector3 ambientLight);

    // AudioSystemにbankデータを流し込む
    void LoadAudioBank(const std::string& name);
};

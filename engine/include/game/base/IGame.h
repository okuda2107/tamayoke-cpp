#pragma once

class IGame {
   public:
    IGame() = default;
    virtual ~IGame() = default;

    virtual void IProcessInput(const struct InputStateBase& state) = 0;
    virtual const struct GameDataBase& IUpdate(
        float deltatime, const struct GameMetricsBase& metrics) = 0;
    virtual const struct RenderDataBase& IGenerateRenderData() = 0;
};

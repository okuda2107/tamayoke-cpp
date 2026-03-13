#pragma once
#include "game/base/InputProcessor.h"
#include "input/pose/InputState.h"
#include "renderer/RenderData.h"

class TamayokeGame : public InputProcessor<pose::InputState> {
    class Game* mGame;

   public:
    TamayokeGame();
    ~TamayokeGame();

    bool Initialize();

    void ProcessInput(const pose::InputState& state) override;
};

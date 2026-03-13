#include "Engine.h"
#include "SDL.h"
#include "TamayokeGame.h"
#include "game/object/ActorsSystem.h"
#include "input/pose/InputSystem.h"
#include "renderer/Renderer.h"
#include "runtime/RuntimeSystem.h"

int main() {
    // 設定値
    int cameraNum = 1;
    float screenW = 1024.0f;
    float screenH = 768.0f;

    TamayokeGame* game = nullptr;
    Renderer* renderer = nullptr;
    pose::InputSystem* inputSystem = nullptr;
    RuntimeSystem* runtimeSystem = nullptr;

    try {
        // runtime system
        runtimeSystem = new RuntimeSystem();
        if (!runtimeSystem->Initialize())
            throw std::runtime_error("Failed to initialize runtime system");

        // game
        game = new TamayokeGame();
        if (!game->Initialize())
            throw std::runtime_error("Failed to initialize game");

        // renderer
        renderer = new Renderer();
        if (!renderer->Initialize(screenW, screenH, "tamayoke", false))
            throw std::runtime_error("Failed to initialize renderer");

        // input system
        inputSystem = new pose::InputSystem(screenW, screenH);
        if (!inputSystem->Initialize(cameraNum))
            throw std::runtime_error("Failed to initialize input system");

        // Load Object
        // Load Audio
        game->LoadAudioBank("Assets/Master.bank");
    } catch (const std::runtime_error& e) {
        SDL_Log(e.what());
    }

    delete runtimeSystem;
    delete inputSystem;
    delete renderer;
    delete game;

    SDL_Quit();

    return 0;
}

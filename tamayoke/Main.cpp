#include "ConfigDB.h"
#include "Engine.h"
#include "SDL.h"
#include "TamayokeGame.h"
#include "actor/LeftHand.h"
#include "actor/Pose.h"
#include "actor/RightHand.h"
#include "game/base/IGame.h"
#include "game/object/ActorsSystem.h"
#include "game/physics/PhysWorld.h"
#include "input/pose/InputSystem.h"
#include "renderer/RenderDB.h"
#include "renderer/Renderer.h"
#include "runtime/RuntimeSystem.h"
#include "scene/GameScene.h"
#include "scene/SceneTag.h"
#include "scene/Title.h"

int main(int argc, char** argv) {
    TamayokeGame* game = nullptr;
    Renderer* renderer = nullptr;
    pose::InputSystem* inputSystem = nullptr;
    RuntimeSystem* runtimeSystem = nullptr;

    try {
        // 設定値
        ConfigDB configDB = ConfigDB(1, 1024.0, 768.0);

        // runtime system
        runtimeSystem = new RuntimeSystem();
        if (!runtimeSystem->Initialize())
            throw std::runtime_error("Failed to initialize runtime system");

        // game
        game = new TamayokeGame(configDB);
        if (!game->Initialize())
            throw std::runtime_error("Failed to initialize game");
        IGame* igame = new IGame();
        igame->input = game;
        igame->update = game;
        igame->render = game;

        // renderer
        renderer = new Renderer();
        if (!renderer->Initialize(configDB.screenW, configDB.screenH,
                                  "tamayoke", false))
            throw std::runtime_error("Failed to initialize renderer");

        // input system
        inputSystem = new pose::InputSystem(configDB.screenW, configDB.screenH);
        if (!inputSystem->Initialize(configDB.cameraNum))
            throw std::runtime_error("Failed to initialize input system");

        // Load Object
        // Load Audio
        game->LoadAudioBank("Assets/Master.bank");

        game->GetActorQuery()
            ->CreateActor<Pose, PoseDeps, TypeLists<RenderDB, ConfigDB>>();
        game->GetActorQuery()
            ->CreateActor<RightHand, HandsBaseDeps,
                          TypeLists<RenderDB, PhysWorld, ConfigDB>>();
        game->GetActorQuery()
            ->CreateActor<LeftHand, HandsBaseDeps,
                          TypeLists<RenderDB, PhysWorld, ConfigDB>>();

        // Load Scene
        game->SetScene<Title>(SceneName::title.data());
        game->SetScene<GameScene>(SceneName::game.data());
        // game->SetEntryScene(SceneName::title.data());
        // test
        game->SetEntryScene(SceneName::game.data());

        Engine engine(igame, inputSystem, renderer, runtimeSystem);

        engine.RunLoop();

        delete igame;
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

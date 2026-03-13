#include "TamayokeGame.h"

#include "SDL.h"
#include "game/Game.h"
#include "game/RuntimeRequestManager.h"
#include "game/StateManager.h"
#include "game/audio/AudioSystem.h"
#include "game/physics/PhysWorld.h"
#include "input/InputState.h"
#include "renderer/RenderDB.h"
#include "renderer/RenderData.h"

TamayokeGame::TamayokeGame() {
    auto renderDB = new RenderDB();
    auto audioSystem = new AudioSystem();
    auto physWorld = new PhysWorld();
    auto stateManager = new StateManager();
    auto reqManager = new RuntimeRequestManager();

    mSystems.AddSystem(renderDB);
    mSystems.AddSystem(audioSystem);
    mSystems.AddSystem(physWorld);
    mSystems.AddSystem(stateManager);
    mSystems.AddSystem(reqManager);

    mFrameResult.mIsGameLoop = true;
}

bool TamayokeGame::Initialize() {
    try {  // 初期化に失敗したら初期状態にロールバックを行う
        if (!mSystems.GetSystem<RenderDB>()->Initialize()) {
            throw std::runtime_error("Failed to Initialize RenderDB");
        }

        if (!mSystems.GetSystem<AudioSystem>()->Initialize()) {
            throw std::runtime_error("Failed to initialize audio system");
        }
    } catch (const std::runtime_error& e) {
        SDL_Log(e.what());
        delete mSystems.GetSystem<RenderDB>();
        delete mSystems.GetSystem<AudioSystem>();
        auto renderDB = new RenderDB();
        auto audioSystem = new AudioSystem();
        mSystems.AddSystem(renderDB);
        mSystems.AddSystem(audioSystem);

        return false;
    }

    return true;
}

void TamayokeGame::ProcessInput(const pose::InputState& state) {
    mFrameResult.mIsGameLoop = !state.isQuitGame;
    mSystems.GetSystem<ActorsSystem>()->ProcessInput(state);
}

const GameFrameResult& TamayokeGame::Update(
    float deltatime, const struct GameMetricsBase& metrics) {
    mSystems.GetSystem<AudioSystem>()->Update(deltatime);
    mSystems.GetSystem<ActorsSystem>()->UpdateObjects(deltatime);
    mSceneManager->Update();
    return mFrameResult;
}

const RenderData& TamayokeGame::GenerateRenderData() {
    auto renderDB = mSystems.GetSystem<RenderDB>();
    return renderDB->GetData();
}

void TamayokeGame::LoadAudioBank(const std::string& name) {
    mSystems.GetSystem<AudioSystem>()->LoadBank(name);
}

#include "TamayokeGame.h"

#include "SDL.h"
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

    mSystems.AddSystem(renderDB);
    mSystems.AddSystem(audioSystem);
    mSystems.AddSystem(physWorld);
    mSystems.AddSystem(stateManager);

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

const GameFrameResult& TamayokeGame::Update(float deltatime,
                                            const struct GameMetricsBase& _) {
    GameImplBase::Update();
    mSystems.GetSystem<AudioSystem>()->Update(deltatime);
    mSystems.GetSystem<ActorsSystem>()->UpdateObjects(deltatime);
    return mFrameResult;
}

const RenderData& TamayokeGame::GenerateRenderData() {
    auto renderDB = mSystems.GetSystem<RenderDB>();
    auto uiSystem = mSystems.GetSystem<UISystem>();
    if (!uiSystem->GetUIStack().empty())
        renderDB->SetUI(&uiSystem->GetUIStack());
    else
        renderDB->SetUI(nullptr);
    return renderDB->GetData();
}

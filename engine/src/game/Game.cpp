#include "game/Game.h"

#include "SDL.h"
#include "game/RuntimeRequestManager.h"
#include "game/StateManager.h"
#include "game/audio/AudioSystem.h"
#include "game/physics/PhysWorld.h"
#include "input/InputState.h"
#include "renderer/RenderDB.h"
#include "renderer/RenderData.h"

Game::Game() {
    auto renderDB = new RenderDB();
    auto audioSystem = new AudioSystem();
    auto physWorld = new PhysWorld();
    auto stateManager = new StateManager();
    auto reqManager = new RuntimeRequestManager();
    reqManager->mInputSystemMetricsRequest.mRelativeMouseMode = true;

    mSystems.AddSystem(renderDB);
    mSystems.AddSystem(audioSystem);
    mSystems.AddSystem(physWorld);
    mSystems.AddSystem(stateManager);
    mSystems.AddSystem(reqManager);

    mFrameResult.mIsGameLoop = true;
    mFrameResult.mRelativeMouseMode = true;
}

bool Game::Initialize() {
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

void Game::ProcessInput(const InputState& state) {
    auto iter = state.EventMap.find(SDL_QUIT);
    if (iter != state.EventMap.end()) {
        mFrameResult.mIsGameLoop = false;
    }

    if (state.Keyboard.GetKeyState(SDL_SCANCODE_ESCAPE) == EReleased) {
        mFrameResult.mIsGameLoop = false;
    }

    // 入力に対して，ゲームオブジェクト，UIを反応させる
    if (mSystems.GetSystem<StateManager>()->mState == GameState::EGameplay) {
        mSystems.GetSystem<ActorsSystem>()->ProcessInput(state);
    } else if (!mSystems.GetSystem<UISystem>()->GetUIStack().empty()) {
        mSystems.GetSystem<UISystem>()->GetUIStack().back()->ProcessInput(
            state);
    }
}

const GameFrameResult& Game::Update(float deltatime,
                                    const struct GameMetricsBase& metrics) {
    mSystems.GetSystem<AudioSystem>()->Update(deltatime);
    mSystems.GetSystem<ActorsSystem>()->UpdateObjects(deltatime);
    mSystems.GetSystem<UISystem>()->Update(deltatime);
    mFrameResult.mRelativeMouseMode =
        mSystems.GetSystem<RuntimeRequestManager>()
            ->mInputSystemMetricsRequest.mRelativeMouseMode;
    return mFrameResult;
}

const RenderData& Game::GenerateRenderData() {
    auto renderDB = mSystems.GetSystem<RenderDB>();
    auto uiSystem = mSystems.GetSystem<UISystem>();
    if (!uiSystem->GetUIStack().empty())
        renderDB->SetUI(&uiSystem->GetUIStack());
    else
        renderDB->SetUI(nullptr);
    return renderDB->GetData();
}

void Game::SetAmbientLight(Vector3 ambientLight) {
    mSystems.GetSystem<RenderDB>()->SetAmbientLight(ambientLight);
}

void Game::LoadAudioBank(const std::string& name) {
    mSystems.GetSystem<AudioSystem>()->LoadBank(name);
}

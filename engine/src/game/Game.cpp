#include "game/Game.h"

#include "SDL.h"
#include "game/RuntimeRequestManager.h"
#include "game/StateManager.h"
#include "game/UI/UIScreen.h"
#include "game/UI/UISystem.h"
#include "game/audio/AudioSystem.h"
#include "game/object/ActorsSystem.h"
#include "game/physics/PhysWorld.h"
#include "game/scene/ActorQuery.h"
#include "game/scene/SceneManager.h"
#include "input/InputState.h"
#include "renderer/RenderDB.h"
#include "renderer/RenderData.h"

Game::Game() {
    mActorsSystem = new ActorsSystem();
    mRenderDB = new RenderDB();
    mAudioSystem = new AudioSystem();
    mPhysWorld = new PhysWorld();
    mUISystem = new UISystem();
    mStateManager = new StateManager();
    mReqManager = new RuntimeRequestManager();
    mReqManager->mInputSystemMetricsRequest.mRelativeMouseMode = true;
    mSceneManager = new SceneManager();

    ActorQueryDeps acd = ActorQueryDeps{
        *mActorsSystem, *mRenderDB,     *mAudioSystem, *mPhysWorld,
        *mUISystem,     *mStateManager, *mReqManager,
    };
    mActorQuery = new ActorQuery(acd);

    mFrameResult.mIsGameLoop = true;
    mFrameResult.mRelativeMouseMode = true;
}

Game::~Game() {
    // ゲームオブジェクト管理類はsystem群に大きく依存しているので，先にdeleteする必要がある．
    delete mActorsSystem;
    delete mSceneManager;
    delete mUISystem;
    delete mActorQuery;
    delete mPhysWorld;
    delete mAudioSystem;
    delete mRenderDB;
    delete mStateManager;
    delete mReqManager;
}

bool Game::Initialize() {
    try {  // 初期化に失敗したら初期状態にロールバックを行う
        if (!mRenderDB->Initialize()) {
            throw std::runtime_error("Failed to Initialize RenderDB");
        }

        if (!mAudioSystem->Initialize()) {
            throw std::runtime_error("Failed to initialize audio system");
        }
    } catch (const std::runtime_error& e) {
        SDL_Log(e.what());
        delete mRenderDB;
        delete mAudioSystem;
        mRenderDB = new RenderDB();
        mAudioSystem = new AudioSystem();

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
    if (mStateManager->mState == GameState::EGameplay) {
        mActorsSystem->ProcessInput(state);
    } else if (!mUISystem->GetUIStack().empty()) {
        mUISystem->GetUIStack().back()->ProcessInput(state);
    }
}

const GameFrameResult& Game::Update(float deltatime,
                                    const struct GameMetricsBase& metrics) {
    mAudioSystem->Update(deltatime);
    mActorsSystem->UpdateObjects(deltatime);
    mUISystem->Update(deltatime);
    mFrameResult.mRelativeMouseMode =
        mReqManager->mInputSystemMetricsRequest.mRelativeMouseMode;
    mSceneManager->Update();
    return mFrameResult;
}

const RenderData& Game::GenerateRenderData() {
    if (!mUISystem->GetUIStack().empty())
        mRenderDB->SetUI(&mUISystem->GetUIStack());
    else
        mRenderDB->SetUI(nullptr);
    return mRenderDB->GetData();
}

void Game::SetAmbientLight(Vector3 ambientLight) {
    mRenderDB->SetAmbientLight(ambientLight);
}

void Game::LoadAudioBank(const std::string& name) {
    mAudioSystem->LoadBank(name);
}

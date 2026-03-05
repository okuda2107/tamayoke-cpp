#pragma once
#include <type_traits>

#include "base/GameBase.h"
#include "game/UI/UIScreen.h"
#include "game/object/Actor.h"
#include "game/scene/ActorQuery.h"
#include "input/InputState.h"
#include "renderer/RenderData.h"
#include "runtime/RuntimeData.h"
#include "scene/Scene.h"
#include "scene/SceneManager.h"

class Game : public GameBase<InputState, RenderData, GameFrameResult,
                             GameMetricsBase> {
    class ActorsSystem* mActorsSystem;
    class RenderDB* mRenderDB;
    class AudioSystem* mAudioSystem;
    class PhysWorld* mPhysWorld;
    class UISystem* mUISystem;

    class StateManager* mStateManager;

    class SceneManager* mSceneManager;
    class ActorQuery* mActorQuery;

    GameFrameResult mFrameResult;
    class RuntimeRequestManager* mReqManager;

   public:
    Game();
    virtual ~Game();

    bool Initialize();

    void ProcessInput(const InputState& state) override;
    const struct GameFrameResult& Update(
        float deltatime, const struct GameMetricsBase& metrics) override;
    const struct RenderData& GenerateRenderData() override;

    template <typename TScene>
    bool SetScene(const std::string& tag) {
        static_assert(std::is_base_of<Scene, TScene>::value,
                      "TScene must derive from Scene");
        TScene* scene = new TScene();
        scene->SetActorQuery(mActorQuery);
        scene->SetDataRef(&mSceneManager->mData);
        return mSceneManager->SetScene(tag, scene);
    }

    bool SetEntryScene(const std::string& tag) {
        return mSceneManager->SetEntryScene(tag);
    }

    class ActorQuery* GetActorQuery() { return mActorQuery; }

    // RenderDBに環境光のデータを流し込む
    void SetAmbientLight(Vector3 ambientLight);

    // AudioSystemにbankデータを流し込む
    void LoadAudioBank(const std::string& name);
};

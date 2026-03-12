#pragma once
#include "GameBase.h"
#include "game/UI/UISystem.h"
#include "game/object/ActorsSystem.h"
#include "game/scene/ActorQuery.h"
#include "game/scene/Scene.h"
#include "game/scene/SceneManager.h"

// ゲーム内部でActor, UI, Scene, ActorQueryを持つことを定義するクラス
// Updateを実装する際に，GameImplBase::Update()処理を差し込む必要がある
/*
    型指定:
    InputState: 入力システムから送られてくる情報を格納する
    RenderData: 描画システムに送る情報を格納する
    GameData: ゲームを更新した際に結果を外部に伝達したいときのコンテナ
    GameMetrics: ゲームを更新する際に必要な外部情報を格納する
*/
template <typename InputState, typename RenderData, typename GameData,
          typename GameMetrics>
class GameImplBase
    : public GameBase<InputState, RenderData, GameData, GameMetrics> {
   protected:
    class SceneManager* mSceneManager;
    class ActorQuery* mActorQuery;

   public:
    GameImplBase() {
        auto actorsSystem = new ActorsSystem();
        auto uiSystem = new UISystem();

        this->mSystems.AddSystem(actorsSystem);
        this->mSystems.AddSystem(uiSystem);

        mSceneManager = new SceneManager();
        mActorQuery = new ActorQuery(ActorQueryDeps{
            *actorsSystem,
            *uiSystem,
            this->mSystems,
        });
    }

    virtual ~GameImplBase() {
        delete mSceneManager;
        delete mActorQuery;
    }

    // GameクラスのUpdateに組み込む必要がある
    void Update() { mSceneManager->Update(); }

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
};

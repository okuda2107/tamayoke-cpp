#pragma once
#include "game/UI/UISystem.h"
#include "game/object/ActorsSystem.h"
#include "game/scene/ActorQuery.h"
#include "game/scene/Scene.h"
#include "game/scene/SceneManager.h"

// ゲーム内部でActor, UI, Scene, ActorQueryを持つことを定義するクラス
// Updateを実装する際に，GameImplBase::Update()処理を差し込む必要がある
class GameBase {
   protected:
    GameSystemVector mSystems;
    class SceneManager* mSceneManager;
    class ActorQuery* mActorQuery;

   public:
    GameBase();
    virtual ~GameBase();

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

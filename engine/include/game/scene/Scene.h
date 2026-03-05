#pragma once
#include <string>
#include <unordered_map>

#include "runtime/RuntimeData.h"

class Scene {
    friend class SceneManager;

   protected:
    class ActorQuery* mActorQuery;

    std::unordered_map<std::string, struct SceneContext>* mSceneManagerData;

    /*
        外部からSceneをロードされ，SceneManagerを通さないことを防ぐため，
        SceneManagerをfriendクラスにして，Load / Unload をprotectedにする．
    */
    // Sceneの有効化
    virtual void LoadActors() = 0;
    // Sceneの無効化
    virtual void UnloadActors() = 0;

   public:
    virtual ~Scene() = default;

    // Gameの状態を監視し，必要なら各Actorに要求を出す
    virtual void TickRules() = 0;

    // SceneManagerに登録されているSceneのtag文字列を返す
    // 遷移しないときは空文字列を返す
    // 存在しないtag文字列を返すと，Logを出して，処理が続行される
    virtual std::string PollNextScene() = 0;

    void SetActorQuery(class ActorQuery* query) { mActorQuery = query; }
    void SetDataRef(
        std::unordered_map<std::string, struct SceneContext>* data) {
        mSceneManagerData = data;
    }
};

#pragma once
#include <string>
#include <unordered_map>

#include "game/object/Actor.h"

struct SceneContext {
    int number;
    std::string str;
    // Sceneを超えて同じActorを使いたいときに格納する
    ActorID actorID;
};

class SceneManager {
    std::string mCurrentSceneTag = "";
    std::unordered_map<std::string, class Scene*> mScenes;

    void UnloadScenes();

   public:
    virtual ~SceneManager();

    void Update();

    bool SetEntryScene(const std::string& tag);
    bool SetScene(const std::string& tag, class Scene* scene);

    // Scene間でデータをやり取りする際に用いる
    std::unordered_map<std::string, SceneContext> mData;
};

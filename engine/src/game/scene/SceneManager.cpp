#include "game/scene/SceneManager.h"

#include "SDL.h"
#include "game/scene/Scene.h"

SceneManager::~SceneManager() { UnloadScenes(); }

void SceneManager::UnloadScenes() {
    for (auto i : mScenes) delete i.second;
}

bool SceneManager::SetEntryScene(const std::string& tag) {
    auto iter = mScenes.find(tag);
    if (iter == mScenes.end() && mCurrentSceneTag != "") return false;

    iter->second->LoadActors();

    mCurrentSceneTag = tag;
    return true;
}

// Scene遷移するかを定期問い合わせする．
// mapに無いtagを指定してもSceneは切り替わらない
void SceneManager::Update() {
    if (mCurrentSceneTag == "") return;

    auto currScene = mScenes.at(mCurrentSceneTag);

    currScene->TickRules();

    std::string nextSceneTag = currScene->PollNextScene();

    if (nextSceneTag == "") return;

    auto nextIter = mScenes.find(nextSceneTag);
    if (nextIter == mScenes.end()) {
        SDL_Log("specified undefined scene: %s", nextSceneTag);
        return;
    }

    currScene->UnloadActors();

    nextIter->second->LoadActors();

    mCurrentSceneTag = nextSceneTag;
}

// 指定tagが重複していたら登録せずにfalseを返す
bool SceneManager::SetScene(const std::string& tag, Scene* scene) {
    auto iter = mScenes.find(tag);
    if (iter != mScenes.end()) return false;

    mScenes.emplace(tag, scene);
    return true;
}

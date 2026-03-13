#include "game/base/GameBase.h"

GameBase::GameBase() {
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

GameBase::~GameBase() {
    delete mSceneManager;
    delete mActorQuery;
}

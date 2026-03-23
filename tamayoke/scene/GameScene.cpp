#include "GameScene.h"

#include "../ConfigDB.h"
#include "../actor/Core.h"
#include "SceneTag.h"
#include "game/audio/AudioSystem.h"
#include "game/physics/PhysWorld.h"
#include "game/scene/ActorQuery.h"
#include "renderer/RenderDB.h"

void GameScene::LoadActors() {
    mCoreID =
        mActorQuery->CreateActor<Core, CoreDeps,
                                 TypeLists<RenderDB, PhysWorld, ConfigDB>>();
}

void GameScene::UnloadActors() {
    auto core = mActorQuery->GetActor<Core>(mCoreID);
    if (core) core->SetState(Actor::State::EDead);
    mCoreID = -1;
}

void GameScene::TickRules() {}

std::string GameScene::PollNextScene() { return ""; }

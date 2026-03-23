#include "GameScene.h"

#include "../ConfigDB.h"
#include "../actor/Core.h"
#include "../actor/LeftHandCore.h"
#include "../actor/RightHandCore.h"
#include "SceneTag.h"
#include "game/audio/AudioSystem.h"
#include "game/physics/PhysWorld.h"
#include "game/scene/ActorQuery.h"
#include "renderer/RenderDB.h"

void GameScene::LoadActors() {
    mCoreID =
        mActorQuery->CreateActor<Core, CoreDeps,
                                 TypeLists<RenderDB, PhysWorld, ConfigDB>>();
    mLeftHandCoreID =
        mActorQuery->CreateActor<LeftHandCore, HandsBaseDeps,
                                 TypeLists<RenderDB, PhysWorld, ConfigDB>>();
    mRightHandCoreID =
        mActorQuery->CreateActor<RightHandCore, HandsBaseDeps,
                                 TypeLists<RenderDB, PhysWorld, ConfigDB>>();
}

void GameScene::UnloadActors() {
    auto core = mActorQuery->GetActor<Core>(mCoreID);
    if (core) core->SetState(Actor::State::EDead);
    mCoreID = -1;
    auto left = mActorQuery->GetActor<LeftHandCore>(mLeftHandCoreID);
    if (left) left->SetState(Actor::State::EDead);
    mLeftHandCoreID = -1;
    auto right = mActorQuery->GetActor<RightHandCore>(mRightHandCoreID);
    if (right) right->SetState(Actor::State::EDead);
    mRightHandCoreID = -1;
}

void GameScene::TickRules() {}

std::string GameScene::PollNextScene() {
    auto core = mActorQuery->GetActor<Core>(mCoreID);
    if (!core || core->isFlag) return SceneName::result.data();
    return "";
}

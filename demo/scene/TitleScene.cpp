#include "TitleScene.h"

#include "../UI/BonfireUI.h"
#include "../UI/Title.h"
#include "../actor/Bonfire.h"
#include "../actor/BonfirePlayer.h"
#include "BonfireSceneTag.h"
#include "game/scene/ActorQuery.h"
#include "game/scene/SceneManager.h"

void TitleScene::LoadActors() {
    // タイトル
    mTitleID = mActorQuery->CreateUI<Title, BonfireUIDeps,
                                     TypeLists<RenderDB, StateManager>>();

    // Bonfire
    auto bonfireID =
        mActorQuery->CreateActor<Bonfire, BonfireDeps,
                                 TypeLists<RenderDB, AudioSystem, PhysWorld>>();

    // player
    auto playerID = mActorQuery->CreateActor<
        BonfirePlayer, BonfirePlayerDeps,
        TypeLists<RenderDB, AudioSystem, PhysWorld, UISystem, StateManager>>();

    auto player = mActorQuery->GetActor<BonfirePlayer>(playerID);
    auto bonfire = mActorQuery->GetActor<Bonfire>(bonfireID);
    if (bonfire && player) {
        player->SetFieldRange(mFieldMin, mFieldMax);
        player->SetPitchAngular(-Math::Pi / 4);
        player->SetForwardSpeed(60.0f);
        player->SetStrafeSpeed(60.0f);
        player->SetBonfireID(bonfireID);
        bonfire->SetPlayer(playerID);
    }

    // bonfire と player を別のSceneで使うため保存
    SceneContext playerIDContext{};
    playerIDContext.actorID = playerID;
    mSceneManagerData->emplace(mPlayerIDTag, playerIDContext);

    SceneContext bonfireIDContext{};
    bonfireIDContext.actorID = bonfireID;
    mSceneManagerData->emplace(mBonfireIDTag, bonfireIDContext);
}

void TitleScene::UnloadActors() { mTitleID = -1; }

void TitleScene::TickRules() {}

std::string TitleScene::PollNextScene() {
    auto title = mActorQuery->GetUI<Title>(mTitleID);
    if (title && title->GetState() == UIScreen::EClosing) {
        return SceneName::game.data();
    } else if (!title) {
        return SceneName::game.data();
    }
    return "";
}

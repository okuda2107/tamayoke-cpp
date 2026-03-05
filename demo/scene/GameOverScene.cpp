#include "GameOverScene.h"

#include "../UI/GameOverUI.h"
#include "../actor/Bonfire.h"
#include "../actor/BonfirePlayer.h"
#include "../scene/BonfireSceneTag.h"
#include "game/scene/ActorQuery.h"
#include "game/scene/SceneManager.h"

void GameOverScene::LoadActors() {
    // result
    mResultUIID = mActorQuery->CreateUI<GameOverUI, BonfireUIDeps,
                                        TypeLists<RenderDB, StateManager>>();

    auto ui = mActorQuery->GetUI<GameOverUI>(mResultUIID);
    if (!ui) throw std::runtime_error("Invalid GameOverUI");

    auto iter = mSceneManagerData->find(mResultTag);
    if (iter == mSceneManagerData->end())
        ui->SetTime(-1);
    else {
        int time = iter->second.number;
        ui->SetTime(time);
    }
}

void GameOverScene::UnloadActors() {
    // Player と Bonfire を取得
    auto playerIDContext = mSceneManagerData->at(mPlayerIDTag);
    auto playerID = playerIDContext.actorID;
    auto bonfireIDContext = mSceneManagerData->at(mBonfireIDTag);
    auto bonfireID = bonfireIDContext.actorID;

    auto player = mActorQuery->GetActor<BonfirePlayer>(playerID);
    auto bonfire = mActorQuery->GetActor<Bonfire>(bonfireID);

    // data reset
    mResultUIID = -1;

    if (player) player->SetState(Actor::State::EDead);
    if (bonfire) bonfire->SetState(Actor::State::EDead);

    mSceneManagerData->clear();
}

void GameOverScene::TickRules() {}

std::string GameOverScene::PollNextScene() {
    auto resultUI = mActorQuery->GetUI<GameOverUI>(mResultUIID);
    if (resultUI && resultUI->GetState() == UIScreen::EClosing) {
        return SceneName::title.data();
    } else if (!resultUI) {
        return SceneName::title.data();
    }
    return "";
}

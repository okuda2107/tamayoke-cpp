#include "Title.h"

#include "../actor/Button.h"
#include "../actor/Logo.h"
#include "SceneTag.h"
#include "game/audio/AudioSystem.h"
#include "game/physics/PhysWorld.h"
#include "game/scene/ActorQuery.h"
#include "renderer/RenderDB.h"

void Title::LoadActors() {
    // ロゴロード
    mLogoID =
        mActorQuery
            ->CreateActor<Logo, LogoDeps, TypeLists<RenderDB, AudioSystem>>();

    mButtonID =
        mActorQuery
            ->CreateActor<Button, ButtonDeps, TypeLists<RenderDB, PhysWorld>>();
}

void Title::UnloadActors() {
    auto logo = mActorQuery->GetActor<Logo>(mLogoID);
    if (logo) logo->SetState(Actor::State::EDead);
    mLogoID = -1;
    auto button = mActorQuery->GetActor<Button>(mButtonID);
    if (button) button->SetState(Actor::State::EDead);
    mButtonID = -1;
}

void Title::TickRules() {}

std::string Title::PollNextScene() {
    auto button = mActorQuery->GetActor<Button>(mButtonID);
    if (!button || button->isFlag) return SceneName::game.data();
    return "";
}

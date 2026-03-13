#include "Title.h"

#include "../actor/Logo.h"
#include "game/audio/AudioSystem.h"
#include "game/scene/ActorQuery.h"
#include "renderer/RenderDB.h"

void Title::LoadActors() {
    // ロゴロード
    mActorQuery
        ->CreateActor<Logo, LogoDeps, TypeLists<RenderDB, AudioSystem>>();
}

void Title::UnloadActors() {}

void Title::TickRules() {}

std::string Title::PollNextScene() { return ""; }

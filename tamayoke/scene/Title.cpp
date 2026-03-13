#include "Title.h"

#include "../actor/Button.h"
#include "../actor/Logo.h"
#include "game/audio/AudioSystem.h"
#include "game/physics/PhysWorld.h"
#include "game/scene/ActorQuery.h"
#include "renderer/RenderDB.h"

void Title::LoadActors() {
    // ロゴロード
    mActorQuery
        ->CreateActor<Logo, LogoDeps, TypeLists<RenderDB, AudioSystem>>();

    mActorQuery
        ->CreateActor<Button, ButtonDeps, TypeLists<RenderDB, PhysWorld>>();
}

void Title::UnloadActors() {}

void Title::TickRules() {}

std::string Title::PollNextScene() { return ""; }

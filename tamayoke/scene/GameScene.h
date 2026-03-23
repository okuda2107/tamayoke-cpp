#pragma once
#include "game/object/Actor.h"
#include "game/scene/Scene.h"

class GameScene : public Scene {
    ActorID mButtonID;
    ActorID mLogoID;

   public:
    void LoadActors();
    void UnloadActors();

    void TickRules();

    std::string PollNextScene();
};

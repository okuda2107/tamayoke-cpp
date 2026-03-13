#pragma once
#include "game/scene/Scene.h"

class Title : public Scene {
   public:
    void LoadActors();
    void UnloadActors();

    void TickRules();

    std::string PollNextScene();
};

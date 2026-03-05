#pragma once
#include "BonfireSceneBase.h"
#include "game/UI/UIScreen.h"

class GameOverScene : public BonfireSceneBase {
    UIID mResultUIID = -1;

   public:
    void LoadActors();
    void UnloadActors();

    void TickRules();

    std::string PollNextScene();
};

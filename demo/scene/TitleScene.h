#pragma once
#include "BonfireSceneBase.h"
#include "game/UI/UIScreen.h"

class TitleScene : public BonfireSceneBase {
    UIID mTitleID = -1;

   public:
    void LoadActors();
    void UnloadActors();

    void TickRules();

    std::string PollNextScene();
};

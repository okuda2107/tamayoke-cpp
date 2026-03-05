#pragma once
#include <string>

#include "core/Math.h"
#include "game/scene/Scene.h"

class BonfireSceneBase : public Scene {
   protected:
    Vector2 mFieldMin;
    Vector2 mFieldMax;

    std::string mPlayerIDTag;
    std::string mBonfireIDTag;
    std::string mResultTag;

   public:
    BonfireSceneBase();
};

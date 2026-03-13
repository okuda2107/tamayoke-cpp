#pragma once
#include "game/base/GameSystemBase.h"

class ConfigDB : public GameSystemBase {
   public:
    int cameraNum;
    float screenW;
    float screenH;
    ConfigDB(int cameraNum, float screenW, float screenH)
        : cameraNum(cameraNum), screenW(screenW), screenH(screenH) {}
};

#include "BonfireSceneBase.h"

BonfireSceneBase::BonfireSceneBase() {
    mFieldMin = Vector2(-1500, -500);
    mFieldMax = Vector2(500, 1500);
    mPlayerIDTag = "playerID";
    mBonfireIDTag = "bonfireID";
    mResultTag = "result";
}

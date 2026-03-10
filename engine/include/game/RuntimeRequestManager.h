#pragma once
#include "game/base/GameSystemBase.h"
#include "runtime/RuntimeData.h"

class RuntimeRequestManager : public GameSystemBase {
   public:
    InputSystemMetrics mInputSystemMetricsRequest;
};

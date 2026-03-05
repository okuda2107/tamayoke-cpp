#pragma once
#include "base/MetricsBase.h"

struct GameFrameResult : GameDataBase {
    bool mIsGameLoop;  // trueの場合，ゲームループを続行
    bool mRelativeMouseMode;
};

struct InputSystemMetrics : InputSystemMetricsBase {
    // 相対マウスモードを有効 / 無効
    bool mRelativeMouseMode;
};

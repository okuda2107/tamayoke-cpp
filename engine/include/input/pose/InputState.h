#pragma once
#include <array>

#include "../base/InputStateBase.h"

namespace pose {
// 座標系: スクリーン左上を{0, 0}，スクリーン右下を{width, height}
// 検出されなかった場合すべての値を-1とする
struct Keypoint {
    float x;
    float y;
    float confidence;

    static const Keypoint Zero;
};

inline const Keypoint Keypoint::Zero = Keypoint{-1, -1, -1};

struct InputState : InputStateBase {
    // 定数定義
    static constexpr int KEYPOINT_COUNT = 17;
    static constexpr int KEYPOINT_NOSE = 0;
    static constexpr int KEYPOINT_LEFT_EYE = 1;
    static constexpr int KEYPOINT_RIGHT_EYE = 2;
    static constexpr int KEYPOINT_LEFT_EAR = 3;
    static constexpr int KEYPOINT_RIGHT_EAR = 4;
    static constexpr int KEYPOINT_LEFT_SHOULDER = 5;
    static constexpr int KEYPOINT_RIGHT_SHOULDER = 6;
    static constexpr int KEYPOINT_LEFT_ELBOW = 7;
    static constexpr int KEYPOINT_RIGHT_ELBOW = 8;
    static constexpr int KEYPOINT_LEFT_WRIST = 9;
    static constexpr int KEYPOINT_RIGHT_WRIST = 10;
    static constexpr int KEYPOINT_LEFT_HIP = 11;
    static constexpr int KEYPOINT_RIGHT_HIP = 12;
    static constexpr int KEYPOINT_LEFT_KNEE = 13;
    static constexpr int KEYPOINT_RIGHT_KNEE = 14;
    static constexpr int KEYPOINT_LEFT_ANKLE = 15;
    static constexpr int KEYPOINT_RIGHT_ANKLE = 16;

    std::array<Keypoint, KEYPOINT_COUNT> keypoints;
};
}  // namespace pose

#pragma once
#include <array>

#include "../base/InputStateBase.h"

namespace pose {
struct Keypoint {
    float x;
    float y;
    float confidence;
};

struct InputState : InputStateBase {
    static constexpr int KEYPOINT_COUNT = 17;
    std::array<Keypoint, KEYPOINT_COUNT> keypoints;
    bool detected = false;
};
}  // namespace pose

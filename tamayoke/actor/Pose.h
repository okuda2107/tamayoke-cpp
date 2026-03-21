#pragma once
#include "game/object/IActor.h"
#include "input/pose/InputState.h"

struct PoseDeps : ActorDeps {
    class RenderDB& renderDB;

    PoseDeps(class RenderDB& db) : ActorDeps(), renderDB(db) {}
};

class Pose : public IActor<pose::InputState> {
    class RenderDB& db;

    static constexpr int skelton_count = 12;
    static constexpr int skelton[12][2] = {
        {5, 6},  {5, 7},   {7, 9},   {6, 8},   {8, 10},  {5, 11},
        {6, 12}, {11, 12}, {11, 13}, {13, 15}, {12, 14}, {14, 16},
    };
    static constexpr int joint_count = 13;
    static constexpr int joint[13] = {
        pose::InputState::KEYPOINT_NOSE,
        pose::InputState::KEYPOINT_LEFT_SHOULDER,
        pose::InputState::KEYPOINT_RIGHT_SHOULDER,
        pose::InputState::KEYPOINT_LEFT_ELBOW,
        pose::InputState::KEYPOINT_RIGHT_ELBOW,
        pose::InputState::KEYPOINT_LEFT_WRIST,
        pose::InputState::KEYPOINT_RIGHT_WRIST,
        pose::InputState::KEYPOINT_LEFT_HIP,
        pose::InputState::KEYPOINT_RIGHT_HIP,
        pose::InputState::KEYPOINT_LEFT_KNEE,
        pose::InputState::KEYPOINT_RIGHT_KNEE,
        pose::InputState::KEYPOINT_LEFT_ANKLE,
        pose::InputState::KEYPOINT_RIGHT_ANKLE,
    };

   public:
    Pose(class ActorsSystem* system, PoseDeps& deps);

    void ActorInput(const pose::InputState& state);
};

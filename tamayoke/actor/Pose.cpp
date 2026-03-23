#include "Pose.h"

#include "../ConfigDB.h"
#include "input/pose/InputState.h"
#include "input/pose/InputSystem.h"
#include "renderer/RenderDB.h"

Pose::Pose(ActorsSystem* system, PoseDeps& deps)
    : IActor(system, deps), db(deps.renderDB), config(deps.configDB) {
    // 線
    for (int i = 0; i < skelton_count; i++) {
        db.AddLine(i, Vector3::Zero, Vector3::Zero, Vector3(1, 1, 1));
    }
    // 頭の円
    db.AddCircle(0, Vector3::Zero, 100, Vector3(1, 1, 1), true);
    // そのほかの円
    for (int i = 1; i < joint_count; i++) {
        db.AddCircle(i, Vector3::Zero, 10, Vector3(1, 1, 1), true);
    }
}

void Pose::ActorInput(const pose::InputState& state) {
    // 線
    for (int i = 0; i < skelton_count; i++) {
        db.UpdateLine(i, [=](ShapeLine& line) {
            line.start = convertCoordinate(state.keypoints[skelton[i][0]].x,
                                           state.keypoints[skelton[i][0]].y,
                                           config.screenW, config.screenH);
            line.end = convertCoordinate(state.keypoints[skelton[i][1]].x,
                                         state.keypoints[skelton[i][1]].y,
                                         config.screenW, config.screenH);
        });
    }
    // 円
    for (int i = 0; i < joint_count; i++) {
        db.UpdateCircle(i, [=](ShapeCircle& circle) {
            circle.Update(convertCoordinate(state.keypoints[joint[i]].x,
                                            state.keypoints[joint[i]].y,
                                            config.screenW, config.screenH),
                          circle.radius);
        });
    }
}

Vector3 convertCoordinate(float x, float y, float screenW, float screenH) {
    return std::move(Vector3(x - screenW / 2, -y + screenH / 2, 0));
}

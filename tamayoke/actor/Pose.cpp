#include "Pose.h"

#include "renderer/RenderDB.h"

Pose::Pose(ActorsSystem* system, PoseDeps& deps)
    : IActor(system, deps), db(deps.renderDB) {
    // 線，円を初期化
    db.AddLine(0, Vector3(100, 100, 0), Vector3(-100, -100, 0),
               Vector3(1, 1, 1));
    db.AddCircle(0, Vector3(0, 0, 0), 50, Vector3(1, 1, 1), true);
}

void Pose::ActorInput(const pose::InputState& state) {}

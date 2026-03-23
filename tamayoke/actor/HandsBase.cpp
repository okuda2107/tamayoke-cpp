#include "HandsBase.h"

#include "../ConfigDB.h"
#include "Pose.h"
#include "game/physics/Collision.h"
#include "game/physics/PhysWorld.h"
#include "game/physics/SphereComponent.h"
#include "input/pose/InputState.h"
#include "renderer/RenderDB.h"

HandsBase::HandsBase(ActorsSystem* system, HandsBaseDeps& deps, size_t id)
    : IActor(system, deps), db(deps.renderDB), config(deps.configDB), mID(id) {
    db.AddCircle(mID, Vector3::Zero, 100,
                 Vector3(233.0f / 256, 231.0f / 256, 122.0f / 256), true);

    auto sphereComp =
        new SphereComponent(this, "hand", CollisionCompDeps(deps.phys));
    auto sphere = Sphere();
    sphere.mRadius = 100;
    sphereComp->mSphere = sphere;
}

#include "SDL.h"
void HandsBase::ActorInput(const pose::InputState& state, int keypoint_id) {
    db.UpdateCircle(mID, [=](ShapeCircle& circle) {
        circle.Update(convertCoordinate(state.keypoints[keypoint_id].x,
                                        state.keypoints[keypoint_id].y,
                                        config.screenW, config.screenH),
                      circle.radius);
    });
    SetPosition(convertCoordinate(state.keypoints[keypoint_id].x,
                                  state.keypoints[keypoint_id].y,
                                  config.screenW, config.screenH));
}

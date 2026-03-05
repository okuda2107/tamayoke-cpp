#include "Wood.h"

#include "game/object/ActorsSystem.h"
#include "game/physics/SphereComponent.h"
#include "renderer/Mesh.h"
#include "renderer/MeshComponent.h"
#include "renderer/RenderDB.h"
#include "renderer/RenderData.h"

Wood::Wood(class ActorsSystem* system, WoodDeps deps)
    : Actor(system, ActorDeps{}), mTime(0.0f) {
    MeshComponent* mc =
        new MeshComponent(this, &deps.renderDB, RenderConfigID::Translucent);
    mc->SetMesh(deps.renderDB.GetMesh("Assets/Square.gpmesh"));
    mc->SetShader(deps.renderDB.GetShader("Shaders/Phong"));
    mc->AddTextureIndex(0);
    SetScale(30.0f);
    mSphereComp = new SphereComponent(this, SWoodPhysTag.data(),
                                      CollisionCompDeps(deps.physWorld));
    Sphere sphere{};
    sphere.mCenter = GetPosition();
    sphere.mRadius = 50.0f;
    mSphereComp->mSphere = sphere;
}

void Wood::UpdateActor(float deltatime) {
    mTime += deltatime;
    // くるくる回転するアニメーション
    Quaternion q = Quaternion(Vector3::UnitZ, mTime / 5 * Math::TwoPi);
    SetRotation(q);
}

void Wood::SetPosition(Vector3 pos) {
    mSphereComp->mSphere.mCenter = pos;
    Actor::SetPosition(pos);
}

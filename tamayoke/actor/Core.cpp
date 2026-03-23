#include "Core.h"

#include "../ConfigDB.h"
#include "SDL.h"
#include "core/Math.h"
#include "core/Random.h"
#include "game/physics/PhysWorld.h"
#include "game/physics/SphereComponent.h"
#include "renderer/RenderDB.h"

Core::Core(class ActorsSystem* actorsSystem, CoreDeps& deps)
    : Actor(actorsSystem, deps),
      render(deps.render),
      phys(deps.phys),
      config(deps.config),
      mSpeed(250.0f),
      mCircleID(150),
      isFlag(false) {
    auto rand = Random();

    // 座標をランダムで決める
    auto halfWH = Vector2(config.screenW, config.screenH) * 0.25;
    auto pos = rand.GetVector(halfWH * -1, halfWH);
    SetPosition(Vector3(pos.x, pos.y, 0));

    // 進む方向をランダムで決める
    auto rot = rand.GetFloatRange(0.0f, Math::TwoPi);
    SetRotation(Quaternion(Vector3::UnitZ, rot));

    // 当たり判定
    mSphereComp =
        new SphereComponent(this, "core", CollisionCompDeps(deps.phys));
    auto sphere = Sphere();
    sphere.mRadius = 100;
    mSphereComp->mSphere = sphere;

    // 円を描画
    deps.render.AddCircle(mCircleID, GetPosition(), 100,
                          Vector3(255.0f / 256, 83.0f / 256, 182.0f / 256),
                          true);
};

void Core::UpdateActor(float deltatime) {
    float xMin = -config.screenW / 4;
    float xMax = config.screenW / 4;
    float yMin = -config.screenH / 4;
    float yMax = config.screenH / 4;
    SetPosition(GetPosition() + GetForward() * mSpeed * deltatime);
    auto pos = GetPosition();
    if (pos.x <= xMin || pos.x >= xMax) {
        SetRotation(Quaternion::Concatenate(
            GetRotation(), Quaternion(Vector3::UnitY, Math::Pi)));
        if (pos.x <= xMin)
            SetPosition(Vector3(xMin, pos.y, 0));
        else if (pos.x >= xMax)
            SetPosition(Vector3(xMax, pos.y, 0));
    }
    if (pos.y <= yMin || pos.y >= yMax) {
        SetRotation(Quaternion::Concatenate(
            GetRotation(), Quaternion(Vector3::UnitX, Math::Pi)));
        if (pos.y <= yMin)
            SetPosition(Vector3(pos.x, yMin, 0));
        else if (pos.y >= yMax)
            SetPosition(Vector3(pos.x, yMax, 0));
    }

    render.UpdateCircle(mCircleID, [=](ShapeCircle& circle) {
        circle.Update(pos, circle.radius);
    });

    auto vec = phys.GetArray<SphereComponent>("hand");
    assert(vec);
    for (auto c : *vec) {
        if (Intersect(c->mSphere, mSphereComp->mSphere)) {
            isFlag = true;
            break;
        }
    }
}

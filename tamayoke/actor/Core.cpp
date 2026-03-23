#include "Core.h"

#include "../ConfigDB.h"
#include "SDL.h"
#include "core/Math.h"
#include "core/Random.h"
#include "game/physics/PhysWorld.h"
#include "game/physics/SphereComponent.h"
#include "renderer/RenderDB.h"

Core::Core(class ActorsSystem* actorsSystem, CoreDeps& deps)
    : Actor(actorsSystem, deps), config(deps.config) {
    auto rand = Random();

    // 座標をランダムで決める
    auto halfWH = Vector2(config.screenW, config.screenH) * 0.5;
    auto pos = rand.GetVector(halfWH * -1, halfWH);
    SetPosition(Vector3(pos.x, pos.y, 0));

    // 進む方向をランダムで決める
    auto rot = rand.GetFloatRange(0.0f, Math::TwoPi);
    SetRotation(Quaternion(Vector3::UnitZ, rot));

    // 当たり判定
    auto sphereComp =
        new SphereComponent(this, "core", CollisionCompDeps(deps.phys));
    auto sphere = Sphere();
    sphere.mRadius = 100;
    sphereComp->mSphere = sphere;

    // 円を描画
    deps.render.AddCircle(150, GetPosition(), 100,
                          Vector3(255.0f / 256, 83.0f / 256, 182.0f / 256),
                          true);

    // test
    SDL_Log("%f, %f, %f", GetForward().x, GetForward().y, GetForward().z);
};

void Core::UpdateActor(float deltatime) {}

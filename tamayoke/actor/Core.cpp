#include "Core.h"

#include "../ConfigDB.h"
#include "SDL.h"
#include "core/Math.h"
#include "core/Random.h"

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

    // test
    SDL_Log("%f, %f, %f", GetForward().x, GetForward().y, GetForward().z);
};

void Core::UpdateActor(float deltatime) {}

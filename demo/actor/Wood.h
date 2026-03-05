#pragma once

#include "game/object/Actor.h"

struct WoodDeps {
    class RenderDB& renderDB;
    class PhysWorld& physWorld;

    WoodDeps(class RenderDB& render, class PhysWorld& phys)
        : renderDB(render), physWorld(phys) {}
};

class Wood : public Actor {
    float mTime;

    class SphereComponent* mSphereComp;

   public:
    Wood(class ActorsSystem* system, WoodDeps deps);
    ~Wood() = default;

    void UpdateActor(float deltatime) override;

    // 座標を初期化するときに外部で初期化するため，コリジョンのみ即座に反映させるためのオーバーロード
    void SetPosition(Vector3 pos);

    static constexpr std::string_view SWoodPhysTag = "wood";
};

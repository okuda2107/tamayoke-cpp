#pragma once
#include "game/object/Actor.h"

struct ButtonDeps : ActorDeps {
    class RenderDB& renderDB;
    class PhysWorld& physics;

    ButtonDeps(class RenderDB& db, class PhysWorld& phys)
        : ActorDeps(), renderDB(db), physics(phys) {}
};

class Button : public Actor {
    class ButtonAnimComp* mAnimSpriteComp;
    class BoxComponent* mBoxComp;

    class PhysWorld& physSystem;

   public:
    // Scene切り替えのためのフラグ
    bool isFlag = false;

    Button(class ActorsSystem* system, ButtonDeps& _);

    void UpdateActor(float deltatime) override;
};

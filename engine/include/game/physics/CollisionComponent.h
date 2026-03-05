#pragma once
#include "game/object/Component.h"

struct CollisionCompDeps {
    class PhysWorld& physWorld;

    CollisionCompDeps(class PhysWorld& physWorld) : physWorld(physWorld) {}
};

class CollisionComponent : public Component {
   protected:
    class PhysWorld& mPhysWorld;

    std::string mTag;

   public:
    CollisionComponent(class Actor* owner, const std::string& tag,
                       CollisionCompDeps deps, int updateOrder = 100)
        : Component(owner, updateOrder),
          mPhysWorld(deps.physWorld),
          mTag(tag) {};
    virtual ~CollisionComponent() = default;
};

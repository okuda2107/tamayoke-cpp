#pragma once
#include "Collision.h"
#include "CollisionComponent.h"

class SphereComponent : public CollisionComponent {
   public:
    SphereComponent(class Actor* owner, const std::string& tag,
                    CollisionCompDeps deps, int updateOrder = 100);
    ~SphereComponent();

    void OnUpdateWorldTransform() override;

    Sphere mSphere;
};

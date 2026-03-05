#pragma once
#include "MeshComponent.h"

class AnimMeshComponent : public MeshComponent {
    float mTime;

   public:
    AnimMeshComponent(class Actor* owner, class RenderDB* database,
                      RenderConfigID id);
    ~AnimMeshComponent();

    void Update(float deltatime) override { mTime += deltatime; }
    void Draw() override;
};

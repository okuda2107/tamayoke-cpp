#include "renderer/AnimMeshComponent.h"

#include "renderer/Mesh.h"

AnimMeshComponent::AnimMeshComponent(Actor* owner, RenderDB* database,
                                     RenderConfigID id)
    : MeshComponent(owner, database, id), mTime(0.0f) {}

AnimMeshComponent::~AnimMeshComponent() {}

void AnimMeshComponent::Draw() {
    mTextureIndices.clear();
    size_t size = mMesh->GetTextureArraySize();
    size_t index = static_cast<size_t>(size * mTime) % size;
    mTextureIndices.push_back(index);
    MeshComponent::Draw();
}

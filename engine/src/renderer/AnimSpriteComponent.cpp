#include "renderer/AnimSpriteComponent.h"

AnimSpriteComponent::AnimSpriteComponent(class Actor* owner,
                                         class RenderDB* database)
    : SpriteComponent(owner, database), mFPS(0), mTime(0.0) {}

void AnimSpriteComponent::Update(float deltatime) {
    if (mTextures.empty()) return;
    mTime += deltatime;
    mTexture = mTextures[int(mFPS * mTime) % mTextures.size()];
}

// 0-index
void AnimSpriteComponent::SetTexture(size_t index, class Texture* tex) {
    if (mTextures.empty()) mTexture = tex;
    if (index >= mTextures.size()) mTextures.resize(index + 1);
    mTextures[index] = tex;
}

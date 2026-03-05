#include "renderer/SpriteComponent.h"

#include "core/Math.h"
#include "game/object/Actor.h"
#include "renderer/RenderDB.h"
#include "renderer/Renderer.h"
#include "renderer/Shader.h"
#include "renderer/Texture.h"

SpriteComponent::SpriteComponent(class Actor* owner, RenderDB* database,
                                 int drawOrder)
    : Component(owner),
      mDatabase(database),
      mTexHeight(128),
      mTexWidth(128),
      mTexture(nullptr) {
    mDatabase->AddSprite(this);
}

SpriteComponent::~SpriteComponent() { mDatabase->RemoveSprite(this); }

void SpriteComponent::Draw(Shader* shader) {
    Matrix4 scaleMat = Matrix4::CreateScale(
        static_cast<float>(mTexWidth), static_cast<float>(mTexHeight), 1.0f);
    Matrix4 world = scaleMat * mOwner->GetWorldTransform();

    shader->SetMatrixUniform("uWorldTransform", world);

    mTexture->SetActive();

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void SpriteComponent::SetTexture(Texture* tex) {
    mTexture = tex;
    mTexWidth = tex->GetWidth();
    mTexHeight = tex->GetHeight();
}

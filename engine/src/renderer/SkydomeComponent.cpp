#include "renderer/SkydomeComponent.h"

#include "game/object/Actor.h"
#include "renderer/Mesh.h"
#include "renderer/RenderDB.h"
#include "renderer/Renderer.h"
#include "renderer/Shader.h"
#include "renderer/Texture.h"
#include "renderer/VertexArray.h"

SkydomeComponent::SkydomeComponent(Actor* owner, RenderDB* database)
    : Component(owner),
      mDatabase(database),
      mMesh(nullptr),
      mShader(nullptr),
      mTextureIndex(0) {
    mDatabase->AddSkydome(this);
}

SkydomeComponent::~SkydomeComponent() { mDatabase->RemoveSkydome(); }

void SkydomeComponent::Draw(Matrix4 viewProj) {
    if (!mMesh || !mShader) return;
    mShader->SetActive();
    mShader->SetMatrixUniform("uViewProj", viewProj);
    mShader->SetMatrixUniform("uWorldTransform", mOwner->GetWorldTransform());
    Texture* t = mMesh->GetTexture(mTextureIndex);
    if (t) {
        t->SetActive();
    }
    VertexArray* va = mMesh->GetVertexArray();
    va->SetActive();
    // Draw
    glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}

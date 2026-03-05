#include "renderer/MeshComponent.h"

#include <algorithm>

#include "game/object/Actor.h"
#include "renderer/Mesh.h"
#include "renderer/RenderDB.h"
#include "renderer/Shader.h"
#include "renderer/Texture.h"
#include "renderer/VertexArray.h"

MeshComponent::MeshComponent(Actor* owner, class RenderDB* database,
                             RenderConfigID id)
    : Component(owner), mDatabase(database), mMesh(nullptr), mShader(nullptr) {
    mConfigID = id;

    mDatabase->AddMeshComp(id, this);
}

MeshComponent::~MeshComponent() { mDatabase->RemoveMeshComp(mConfigID, this); }

void MeshComponent::Draw() {
    if (mMesh && mShader) {
        // Set the world transform
        //そのオブジェクト特有のパラメータを用いる場合ここに書く
        mShader->SetMatrixUniform("uWorldTransform",
                                  mOwner->GetWorldTransform());
        mShader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());
        // Set the active texture
        for (size_t i = 0; i < mTextureIndices.size(); i++) {
            Texture* t = mMesh->GetTexture(mTextureIndices[i]);
            if (t) {
                t->SetActive(i);
            }
        }
        // Set the mesh's vertex array as active
        VertexArray* va = mMesh->GetVertexArray();
        va->SetActive();
        // Draw
        glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT,
                       nullptr);
    }
}

// void OpenGL::MeshComponent::LoadProperties(const rapidjson::Value& inObj) {
//     Component::LoadProperties(inObj);

//     std::string meshFile;
//     if (JsonHelper::GetString(inObj, "meshFile", meshFile)) {
//         SetMesh(mOwner->GetGame()->GetRenderer()->GetMesh(meshFile));
//     }

//     int idx;
//     if (JsonHelper::GetInt(inObj, "textureIndex", idx)) {
//         mTextureIndex = static_cast<size_t>(idx);
//     }
// }

void MeshComponent::AddTextureIndex(size_t index) {
    mTextureIndices.push_back(index);
}

void MeshComponent::RemoveTextureIndex(size_t index) {
    auto iter =
        std::find(mTextureIndices.begin(), mTextureIndices.end(), index);
    if (iter != mTextureIndices.end()) {
        mTextureIndices.erase(iter);
    }
}

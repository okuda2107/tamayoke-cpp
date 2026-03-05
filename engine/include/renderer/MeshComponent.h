#pragma once
#include "Renderer.h"
#include "game/object/Component.h"

class MeshComponent : public Component {
   public:
    MeshComponent(class Actor* owner, class RenderDB* database,
                  RenderConfigID config);
    virtual ~MeshComponent();
    // Draw this mesh component
    virtual void Draw();
    // Set the mesh / shader / texture index used by mesh component
    virtual void SetMesh(class Mesh* mesh) { mMesh = mesh; }
    virtual void SetShader(class Shader* shader) { mShader = shader; }
    void AddTextureIndex(size_t index);
    void RemoveTextureIndex(size_t index);

    class Shader* GetShader() { return mShader; }

    // void LoadProperties(const rapidjson::Value& inObj) override;

   protected:
    class RenderDB* mDatabase;

    class Mesh* mMesh;
    class Shader* mShader;
    // このmeshで用いるテクスチャ群
    std::vector<size_t> mTextureIndices;

    RenderConfigID mConfigID;
};

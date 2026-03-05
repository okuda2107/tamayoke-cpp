#include "renderer/RenderDB.h"

#include <algorithm>

#include "SDL.h"
#include "game/UI/Font.h"
#include "renderer/Mesh.h"
#include "renderer/MeshComponent.h"
#include "renderer/RenderData.h"
#include "renderer/Shader.h"
#include "renderer/SpriteComponent.h"
#include "renderer/Texture.h"

RenderDB::~RenderDB() { UnloadAssets(); }

bool RenderDB::Initialize() {
    if (TTF_Init() != 0) {
        SDL_Log("Failed to initialize SDL_ttf");
        return false;
    }

    // initialize data
    mData.mView =
        Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);

    return true;
}

void RenderDB::UnloadAssets() {
    // Destroy textures
    for (auto i : mTextures) {
        i.second->Unload();
        delete i.second;
    }
    mTextures.clear();

    // Destroy meshes
    for (auto i : mMeshes) {
        i.second->Unload();
        delete i.second;
    }
    mMeshes.clear();

    // Destroy shaders
    for (auto i : mShaders) {
        i.second->Unload();
        delete i.second;
    }
    mShaders.clear();

    // Destroy fonts
    for (auto i : mFonts) {
        i.second->Unload();
        delete i.second;
    }
    mFonts.clear();
}

void RenderDB::AddSprite(SpriteComponent* sprite) {
    auto iter = mData.mSprites.begin();
    for (; iter != mData.mSprites.end(); ++iter) {
        if ((*iter)->GetDrawOrder() > sprite->GetDrawOrder()) {
            break;
        }
    }
    mData.mSprites.insert(iter, sprite);
}

void RenderDB::RemoveSprite(SpriteComponent* sprite) {
    auto iter = std::find(mData.mSprites.begin(), mData.mSprites.end(), sprite);
    mData.mSprites.erase(iter);
}

void RenderDB::AddMeshComp(const RenderConfigID id, MeshComponent* mesh) {
    auto iter = mData.mMeshComps.find(id);
    if (iter != mData.mMeshComps.end()) {
        iter->second.push_back(mesh);
    } else {
        mData.mMeshComps.emplace(id, std::vector<MeshComponent*>{mesh});
    }
}

void RenderDB::RemoveMeshComp(const RenderConfigID id, MeshComponent* mesh) {
    auto vectorIter = mData.mMeshComps.find(id);
    if (vectorIter != mData.mMeshComps.end()) {
        auto meshIter = std::find(vectorIter->second.begin(),
                                  vectorIter->second.end(), mesh);
        if (meshIter != vectorIter->second.end())
            vectorIter->second.erase(meshIter);
    }
}

Texture* RenderDB::GetTexture(const std::string& filename) {
    Texture* tex = nullptr;
    auto iter = mTextures.find(filename);
    if (iter != mTextures.end()) {
        tex = iter->second;
    } else {
        tex = new Texture();
        if (tex->Load(filename)) {
            mTextures.emplace(filename, tex);
        } else {
            delete tex;
            tex = nullptr;
        }
    }
    return tex;
}

Mesh* RenderDB::GetMesh(const std::string& fileName) {
    Mesh* m = nullptr;
    auto iter = mMeshes.find(fileName);
    if (iter != mMeshes.end()) {
        m = iter->second;
    } else {
        m = new Mesh();
        if (m->Load(fileName, this)) {
            mMeshes.emplace(fileName, m);
        } else {
            delete m;
            m = nullptr;
        }
    }
    return m;
}

Shader* RenderDB::GetShader(const std::string& shaderName) {
    Shader* m = nullptr;
    auto iter = mShaders.find(shaderName);
    if (iter != mShaders.end()) {
        m = iter->second;
    } else {
        m = new Shader();
        if (m->Load(shaderName + ".vert", shaderName + ".frag")) {
            mShaders.emplace(shaderName, m);
        } else {
            delete m;
            m = nullptr;
        }
    }
    return m;
}

Font* RenderDB::GetFont(const std::string& fileName) {
    auto iter = mFonts.find(fileName);
    if (iter != mFonts.end()) {
        return iter->second;
    } else {
        Font* font = new Font();
        if (font->Load(fileName)) {
            mFonts.emplace(fileName, font);
        } else {
            font->Unload();
            delete font;
            font = nullptr;
        }
        return font;
    }
}

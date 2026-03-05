#include "game/UI/UIScreen.h"

#include <functional>
#include <string>

#include "GL/glew.h"
#include "game/UI/Font.h"
#include "game/UI/UISystem.h"
#include "renderer/Shader.h"
#include "renderer/Texture.h"

size_t HashText::operator()(const TextKey& k) const noexcept {
    size_t hash = std::hash<std::string>()(k.text);
    hash ^= std::hash<int>()(k.size) << 1;
    hash ^= std::hash<uint8_t>()(k.color.x) << 2;
    hash ^= std::hash<uint8_t>()(k.color.y) << 3;
    hash ^= std::hash<uint8_t>()(k.color.z) << 4;
    return hash;
}

UIScreen::UIScreen(UISystem* system, UIDeps deps)
    : mSystem(system), mState(EActive), mFont(nullptr) {
    // Add to UI Stack
    mID = mSystem->PushUI(this);
}

UIScreen::~UIScreen() {
    for (auto& [name, tex] : mFontTexture) {
        tex->Unload();
    }
    mFontTexture.clear();
}

void UIScreen::DrawTexture(Shader* shader, Texture* texture,
                           const Vector2& offset, float scale) {
    // Scale the quad by the width/height of texture
    Matrix4 scaleMat = Matrix4::CreateScale(
        static_cast<float>(texture->GetWidth()) * scale,
        static_cast<float>(texture->GetHeight()) * scale, 1.0f);
    // Translate to position on screen
    Matrix4 transMat =
        Matrix4::CreateTranslation(Vector3(offset.x, offset.y, 0.0f));
    // Set world transform
    Matrix4 world = scaleMat * transMat;
    shader->SetMatrixUniform("uWorldTransform", world);
    // Set current texture
    texture->SetActive();
    // Draw quad

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

Texture* UIScreen::GetFontTexture(const TextKey& key) {
    if (!mFont) return nullptr;
    auto iter = mFontTexture.find(key);
    if (iter != mFontTexture.end()) {
        return iter->second;
    } else {
        Texture* tex = new Texture();
        tex = mFont->RenderText(key.text, key.color, key.size);
        mFontTexture.emplace(key, tex);
        return tex;
    }
}

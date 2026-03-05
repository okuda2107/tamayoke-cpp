#include "BonfireUI.h"

#include "game/UI/Font.h"
#include "game/UI/UISystem.h"
#include "renderer/RenderDB.h"
#include "renderer/Shader.h"
#include "renderer/Texture.h"

BonfireUI::BonfireUI(UISystem* system, BonfireUIDeps& bud)
    : UIScreen(system, bud), mStateManager(bud.stateManager), mTitle(nullptr) {
    SetFont(bud.renderDB.GetFont("Assets/SlacksideOne-Regular.ttf"));
}

void BonfireUI::SetTitle(const std::string& text, const Vector3& color,
                         int pointSize) {
    // 古い文字テクスチャのリソースを解放してから新しい文字テクスチャを取得する
    if (mTitle) {
        mTitle->Unload();
        delete mTitle;
        mTitle = nullptr;
    }
    mTitle = mFont->RenderText(text, color, pointSize);
}

void BonfireUI::Draw(Shader* shader) {
    if (mTitle) {
        DrawTexture(shader, mTitle, mTitlePos);
    }
}

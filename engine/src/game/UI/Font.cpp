#include "game/UI/Font.h"

#include <vector>

#include "renderer/Renderer.h"
#include "renderer/Texture.h"

Font::Font() {}

Font::~Font() {}

bool Font::Load(const std::string& filename) {
    // サポートするフォントサイズ
    std::vector<int> fontSizes = {8,  9,  10, 11, 12, 14, 16, 18, 20, 22,
                                  24, 26, 28, 30, 32, 34, 38, 40, 42, 44,
                                  46, 48, 52, 56, 60, 64, 68, 72};

    // サイズごとにTTF_OpenFontを呼び出す
    for (auto& size : fontSizes) {
        TTF_Font* font = TTF_OpenFont(filename.c_str(), size);
        if (font == nullptr) {
            SDL_Log("failed to load font %s size %d", filename.c_str(), size);
            return false;
        }
        mFontData.emplace(size, font);
    }
    return true;
}

void Font::Unload() {
    for (auto& font : mFontData) {
        TTF_CloseFont(font.second);
    }
}

Texture* Font::RenderText(const std::string& text, const Vector3& color,
                          int pointSize) {
    Texture* texture = nullptr;

    SDL_Color sdlColor;
    sdlColor.r = static_cast<Uint8>(color.x * 255);
    sdlColor.g = static_cast<Uint8>(color.y * 255);
    sdlColor.b = static_cast<Uint8>(color.z * 255);
    sdlColor.a = 255;

    // フォントのサイズに合ったデータを探す
    auto iter = mFontData.find(pointSize);
    if (iter != mFontData.end()) {
        TTF_Font* font = iter->second;
        // blendしながらsurfaceに描画
        SDL_Surface* surf =
            TTF_RenderUTF8_Blended(font, text.c_str(), sdlColor);
        if (surf != nullptr) {
            /*
            SDL_ttfで使われているFreeTypeがSIMD最適化のため，自動で64Bにパディングする．
            これをそのままOpenGLが読むとformat違いを起こす．

            OpenGLが画像を読み込むときに知っているのは“width, height, format”だけで，
            SDL_ttfはwidthとheightを変更せずにpitchのみを変えて，64Bにパディングする為
            行の終わりにpaddingが入ってることを知ることはできない．
            */
            SDL_Surface* converted =
                SDL_ConvertSurfaceFormat(surf, SDL_PIXELFORMAT_ABGR8888, 0);
            if (!converted) {
                SDL_Log("Failed to convert surface format: %s", SDL_GetError());
                return nullptr;
            }

            // surfaceからテクスチャに変換
            texture = new Texture();
            texture->CreateFromSurface(converted);
            SDL_FreeSurface(converted);
            SDL_FreeSurface(surf);
        } else
            SDL_Log("point size is unsupported");
    }
    return texture;
}

#pragma once
#include <string>
#include <unordered_map>

#include "SDL_ttf.h"
#include "core/Math.h"

class Font {
    std::unordered_map<int, TTF_Font*> mFontData;

   public:
    Font();
    ~Font();

    // ファイルのロード / アンロード
    bool Load(const std::string& filename);
    void Unload();

    class Texture* RenderText(const std::string& text,
                              const Vector3& color = Color::White,
                              int pointSize = 30);
};

#pragma once
#include <string>
#include <unordered_map>

#include "core/Math.h"
#include "game/UI/UISystem.h"

struct TextKey {
    std::string text;
    Vector3 color;
    int size;
    // fontデータをkeyに入れてもいいが，現在はポインタ型でしか扱っていないため不安定になる．
    // fontデータを扱うなら，font名の文字列をkeyとしたい．
    // Font font;

    bool operator==(const TextKey& k) const {
        return text == k.text && size == k.size && color.x == k.color.x &&
               color.y == k.color.y && color.x == k.color.z;
    }
};

struct HashText {
    size_t operator()(const TextKey& k) const noexcept;
};

struct UIDeps {};

/* Note:
    UIScreenの具体クラスのコンストラクタの引数は必ず
    ConcreteUI(class UISystem* system, ConcreteUIDeps deps);
    としないと，ActorFactoryのCreateUIでの生成に失敗する．
*/
class UIScreen {
   public:
    // ClosingだとUIに対して操作できない
    enum UIState { EActive, EClosing };

   protected:
    class UISystem* mSystem;
    UIID mID;

    UIState mState;

    class Font* mFont;
    // フォントをテクスチャに変換した結果をキャッシュする
    std::unordered_map<TextKey, class Texture*, HashText> mFontTexture;

    void DrawTexture(class Shader* shader, class Texture* texture,
                     const Vector2& offset = Vector2::Zero, float scale = 1.0f);

   public:
    UIScreen(class UISystem* system, UIDeps deps);
    virtual ~UIScreen();

    // 派生クラスで処理を決められる
    virtual void Update(float deltaTime) {};
    virtual void Draw(class Shader* shader) = 0;
    virtual void ProcessInput(const class InputState& state) {};

    void SetFont(class Font* font) { mFont = font; }

    class Texture* GetFontTexture(const TextKey& key);

    //状態をClosingにする
    void Close() { mState = EClosing; }
    UIState GetState() const { return mState; }
    UIID GetID() const { return mID; }
};

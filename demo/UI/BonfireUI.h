#pragma once
#include "game/StateManager.h"
#include "game/UI/UIScreen.h"
#include "renderer/RenderDB.h"

struct BonfireUIDeps : UIDeps {
    class RenderDB& renderDB;
    class StateManager& stateManager;

    BonfireUIDeps(RenderDB& renderDB, StateManager& stateManager)
        : UIDeps(), renderDB(renderDB), stateManager(stateManager) {}
};

// UIScreenとChillut's FireのUI実装とを嚙合わせる前のAdapterクラス
class BonfireUI : public UIScreen {
   protected:
    class Texture* mTitle;
    Vector2 mTitlePos;

    class StateManager& mStateManager;

   public:
    BonfireUI(class UISystem* system, BonfireUIDeps& bud);

    void SetTitle(const std::string& text, const Vector3& color = Color::White,
                  int pointSize = 40);

    void Draw(class Shader* shader) override;
};

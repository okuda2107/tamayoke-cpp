#pragma once
#include "game/base/GameBase.h"
#include "game/base/IGame.h"
#include "input/base/IInputSystem.h"
#include "input/base/InputSystemBase.h"
#include "renderer/base/IRenderer.h"
#include "renderer/base/RendererBase.h"
#include "runtime/base/IRuntimeSystem.h"
#include "runtime/base/RuntimeSystemBase.h"

// 入力，更新，出力の連携を責務とする
class Engine {
    // game内部の世界を表現
    class IGame* mGame;
    // 入力情報処理を責務とする
    class IInputSystem* mInputSystem;
    // gameの世界を表示することを責務とする
    class IRenderer* mRenderer;

    class IRuntimeSystem* mRuntimeSystem;

   public:
    // 型指定の正当性を保証するためにtemplate constractorを設ける
    /*
        InputState: InputStateBaseを基底クラスとした，入力情報を取り扱う構造体
        RenderData: RenderDataBaseを基底クラスとした，Rendererにレンダリングする情報を伝えるための構造体
        GameData: GameDataBaseを基底クラスとした，Gameが実行環境に情報を伝えるための構造体
        Metrics: 実行環境からGameなどのシステムに情報を伝えるための型をMetricsBundleで束ねた構造体
    */
    template <typename InputState, typename RenderData, typename GameData,
              typename Metrics>
    Engine(class GameBase<InputState, RenderData, GameData,
                          typename Metrics::Game>* game,
           class InputSystemBase<InputState, typename Metrics::InputSystem>*
               inputSystem,
           class RendererBase<RenderData>* renderer,
           class RuntimeSystemBase<GameData, Metrics>* runtimeSystem)
        : mGame(game),
          mInputSystem(inputSystem),
          mRenderer(renderer),
          mRuntimeSystem(runtimeSystem) {}

    void RunLoop();
};

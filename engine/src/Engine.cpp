#include "Engine.h"

void Engine::RunLoop() {
    while (mRuntimeSystem->IIsRunning()) {
        // frameの開始
        mRuntimeSystem->IBeginFrame();

        // 入力処理
        mInputSystem->IUpdate();
        mGame->IProcessInput(mInputSystem->IGetState());

        // game更新
        const GameDataBase& gameData = mGame->IUpdate(
            mRuntimeSystem->IGetDeltatime(), mRuntimeSystem->IGetGameMetrics());

        // 画面出力
        mRenderer->IDraw(mGame->IGenerateRenderData());

        // runtimeの更新，情報の集計など
        mRuntimeSystem->IProcessGameData(gameData);

        // metricsの伝播
        mInputSystem->IProcessMetrics(mRuntimeSystem->IGetInputSystemMetrics());

        // frameの終了
        mRuntimeSystem->IEndFrame();
    }
}

#pragma once

class IRuntimeSystem {
   public:
    IRuntimeSystem() = default;
    virtual ~IRuntimeSystem() = default;

    // gameの更新を続行するか
    virtual bool IIsRunning() const = 0;

    // フレームの開始，終了を明示
    virtual void IBeginFrame() = 0;
    virtual void IEndFrame() = 0;

    // gameから送られてくるデータを基に処理する
    // ex. ゲームループの継続判定など
    virtual void IProcessGameData(const struct GameDataBase& data) = 0;

    virtual float IGetDeltatime() const = 0;

    // Metricsを出力
    virtual const struct GameMetricsBase& IGetGameMetrics() const = 0;
    virtual const struct InputSystemMetricsBase& IGetInputSystemMetrics()
        const = 0;
};

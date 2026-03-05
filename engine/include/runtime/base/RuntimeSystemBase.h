#pragma once
#include <type_traits>

#include "IRuntimeSystem.h"

template <typename GameView, typename InputView>
struct MetricsBundle {
    using Game = GameView;
    using InputSystem = InputView;
};

// ゲームプログラムの実行環境，フレーム，時間処理を責務とする
// ゲームからの要求に基づき，実行環境に関わる設定変更を各システムへ反映する
template <typename GameData, typename Metrics>
class RuntimeSystemBase : public IRuntimeSystem {
    static_assert(std::is_base_of<GameDataBase, GameData>::value,
                  "GameData must derive from GameDataBase");
    static_assert(
        std::is_base_of<GameMetricsBase, typename Metrics::Game>::value,
        "Metrics::Game must derive from GameMetricsBase");

    static_assert(
        std::is_base_of<InputSystemMetricsBase,
                        typename Metrics::InputSystem>::value,
        "Metrics::InputSystem must derive from InputSystemMetricsBase");

   protected:
    float mDeltatime;

    typename Metrics::Game mGameMetrics;
    typename Metrics::InputSystem mInputSystemMetrics;

   public:
    RuntimeSystemBase() = default;
    virtual ~RuntimeSystemBase() = default;

    // gameの更新を続行するか
    virtual bool IsRunning() const = 0;

    // フレームの開始，終了を明示
    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;

    // gameから送られてくるデータを基に処理する
    // ex. ゲームループの継続判定など
    virtual void ProcessGameData(const GameData& data) = 0;

    // Engineで使われるためのエントリポイント
    bool IIsRunning() const final { return IsRunning(); }

    void IBeginFrame() final { BeginFrame(); }
    void IEndFrame() final { EndFrame(); }

    void IProcessGameData(const struct GameDataBase& data) final {
        ProcessGameData(static_cast<const GameData&>(data));
    }

    float IGetDeltatime() const final { return mDeltatime; }

    const struct GameMetricsBase& IGetGameMetrics() const final {
        return mGameMetrics;
    }
    const struct InputSystemMetricsBase& IGetInputSystemMetrics() const final {
        return mInputSystemMetrics;
    }
};

/*
    このクラスはまだ成長段階で，将来ここに機能が追加されて，Engine側の処理にも組み込まれる可能性がある．
    将来的にはデバッグ機能や，パフォーマンスの表示なども責務としたいが，
    現在の理解と，プロジェクトの規模では，どのような機能が必要で，どのようなフェーズで処理されるかがわからない．
    そのため，Baseクラスであるが，拡張される可能性があるということだけ明示しておく．
    考えられるのは，
    - frame外でRuntimeSystemの更新を行う機能
        - frame内で更新を行うのと，frame外で更新を行うのでは，意味的に差があるので，それを追加するかどうか
    - パフォーマンスの表示などを行う機能をこのクラスに実装？
        - rendererかと思われるかもしれないが，rendererはゲーム結果の表示を責務としており，パフォーマンスなどの表示は
    - ゲームオブジェクトのホットリロード対応
        - ホットリロードに関しては，まだ詳細にどのように実装したらいいかは浮かんでないのでわからないが，実行環境の操作という文脈ではこのクラスで処理することもある気がする
*/

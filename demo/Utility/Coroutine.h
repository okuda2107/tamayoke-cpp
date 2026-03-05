#pragma once
#include <functional>
#include <vector>

// 実験的に導入したものなので，demoディレクトリに配置する

// フレームを超えて実行する処理を管理する
class Coroutine {
    float mTime;

    struct Entry {
        float start;
        float end;
        std::function<void(float)> func;
    };

    // 実行中のコルーチン配列
    // 修了時間を過ぎると，処理を破棄
    // 終了時間順にソート
    std::vector<Entry> mCoroutines;

    // 探索のための補助関数
    static bool CompareEntryEndTime(const Entry& a, const Entry& b) {
        return a.end < b.end;
    }

   public:
    Coroutine();

    // 処理とそれを実行する秒数を指定
    // 渡す関数の引数のfloatは経過割合を受け取る
    // 登録されたfloat値を返し，処理が終わったかどうかの確認に用いる
    float AddCoroutine(float duration, std::function<void(float)> func);
    void Update(float deltatime);

    // コルーチン処理が終了したかどうか
    bool CheckCoroutine(float endTime);
};

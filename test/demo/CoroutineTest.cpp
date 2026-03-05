#include <gtest/gtest.h>

#include "Coroutine.h"

// ------------------------------------------------------------
// コルーチンの基本動作テスト
// ------------------------------------------------------------
TEST(CoroutineTest, SimpleProgression) {
    Coroutine cor;
    float lastT = -1.0f;
    bool finished = false;

    // 1秒間の処理を登録
    float endTime = cor.AddCoroutine(1.0f, [&](float t) {
        lastT = t;
        if (t >= 1.0f) finished = true;
    });

    // 最初の時点ではまだ開始直後
    cor.Update(0.0f);
    EXPECT_FLOAT_EQ(lastT,
                    0.0f);  // 経過割合は0のはず（初回呼び出しなしなら後述）

    // 0.5秒経過させる
    cor.Update(0.5f);
    EXPECT_NEAR(lastT, 0.5f, 1e-5);
    EXPECT_FALSE(cor.CheckCoroutine(endTime));

    // 残り0.5秒進める
    cor.Update(0.5f);
    EXPECT_NEAR(lastT, 1.0f, 1e-5);
    EXPECT_TRUE(finished);
    EXPECT_TRUE(cor.CheckCoroutine(endTime));
}

// ------------------------------------------------------------
// 複数コルーチンの重複動作テスト
// ------------------------------------------------------------
TEST(CoroutineTest, MultipleCoroutines) {
    Coroutine cor;
    float v1 = 0.0f;
    float v2 = 0.0f;

    float end1 = cor.AddCoroutine(1.0f, [&](float t) { v1 = t; });
    float end2 = cor.AddCoroutine(2.0f, [&](float t) { v2 = t; });

    cor.Update(0.5f);
    EXPECT_NEAR(v1, 0.5f, 1e-5);
    EXPECT_NEAR(v2, 0.25f, 1e-5);

    cor.Update(0.5f);
    EXPECT_NEAR(v1, 1.0f, 1e-5);  // 終了済み
    EXPECT_NEAR(v2, 0.5f, 1e-5);
    EXPECT_TRUE(cor.CheckCoroutine(end1));
    EXPECT_FALSE(cor.CheckCoroutine(end2));

    cor.Update(1.0f);
    EXPECT_NEAR(v2, 1.0f, 1e-5);
    EXPECT_TRUE(cor.CheckCoroutine(end2));
}

// ------------------------------------------------------------
// 完了後に削除されているか確認
// ------------------------------------------------------------
TEST(CoroutineTest, Cleanup) {
    Coroutine cor;
    bool called = false;

    cor.AddCoroutine(0.5f, [&](float t) {
        if (t >= 1.0f) called = true;
    });

    // 1秒経過後、mCoroutinesが空であることを確認
    cor.Update(1.0f);
    EXPECT_TRUE(called);

    // もう一度Updateしても呼ばれない
    called = false;
    cor.Update(1.0f);
    EXPECT_FALSE(called);
}

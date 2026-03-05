#include "Coroutine.h"

#include <algorithm>

Coroutine::Coroutine() : mTime(0.0f) {}

float Coroutine::AddCoroutine(float duration, std::function<void(float)> func) {
    float endTime = mTime + duration;
    Entry e;
    e.start = mTime;
    e.end = endTime;
    e.func = std::move(func);
    // 計算量削減のため，まず一番後ろの要素の時間と比較して，それより時間が大きいなら後ろに追加．
    // そうでなければ，時間別にソートしながら挿入．
    if (mCoroutines.empty() || mCoroutines.back().end < e.end)
        mCoroutines.push_back(std::move(e));
    else {
        auto iter = std::lower_bound(mCoroutines.begin(), mCoroutines.end(), e,
                                     CompareEntryEndTime);
        mCoroutines.insert(iter, std::move(e));
    }
    return endTime;
}

void Coroutine::Update(float deltatime) {
    // 時間更新
    mTime += deltatime;

    // 処理
    auto iter =
        std::upper_bound(mCoroutines.begin(), mCoroutines.end(),
                         Entry{0.0f, mTime, nullptr}, CompareEntryEndTime);
    for (auto finish = mCoroutines.begin(); finish < iter; ++finish) {
        finish->func(1.0f);
    }
    for (auto it = iter; it < mCoroutines.end(); ++it) {
        float t = (mTime - it->start) / (it->end - it->start);
        it->func(t);
    }

    // 配列の更新
    mCoroutines.erase(mCoroutines.begin(), iter);
}

bool Coroutine::CheckCoroutine(float endTime) { return mTime >= endTime; }

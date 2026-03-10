#pragma once
#include <vector>

#include "GameSystemBase.h"

namespace SystemIDGenerator {
// 関数が呼ばれるごとに静的変数を1つずつ上げていく関数
inline size_t Next() {
    static size_t counter = 0;
    return counter++;
}
}  // namespace SystemIDGenerator

// 静的変数によって型ごとに一意な値を持つ役割
/*
    template関数であるので，型ごとに関数が作られる．
    静的変数にすることで，初めて実行される関数はidを初期化され，
    二回目以降に実行される関数はその関数独自のidが返される仕組み
    そのため，静的変数は関数ごとに一意な値を持つが，template関数にすることで，型ごとに一意な値を持つことができる
*/
template <class T>
size_t SystemID() {
    static size_t id = SystemIDGenerator::Next();
    return id;
}

// ゲームで用いるシステムをまとめて管理するクラス
class GameSystemVector {
    std::vector<GameSystemBase*> mData;

   public:
    ~GameSystemVector() {
        for (auto data : mData) delete data;
    }

    template <typename TSystem>
    void AddSystem(TSystem* system) {
        static_assert(std::is_base_of<GameSystemBase, TSystem>::value,
                      "TSystem must derive from GameSystemBase");

        size_t id = SystemID<TSystem>();
        if (id >= mData.size()) mData.resize(id + 1);

        mData[id] = system;
    }

    template <typename TSystem>
    TSystem* GetSystem() {
        static_assert(std::is_base_of<GameSystemBase, TSystem>::value,
                      "TSystem must derive from GameSystemBase");

        size_t id = SystemID<TSystem>();

        if (id >= mData.size()) return nullptr;

        return static_cast<TSystem*>(mData[id]);
    }
};

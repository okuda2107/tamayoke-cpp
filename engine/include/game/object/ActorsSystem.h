#pragma once
#include <cstdint>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "base/ObjectsSystemBase.h"

// Actorを貸し出すときに発行するモノ
// 外部システムはこれを所有し，Actorを借用するときにActorSystemに問いかける
typedef uint32_t ActorID;

// Actorの配列を保持し，それらのupdateを保証する
class ActorsSystem : public ObjectsSystemBase<class InputState> {
    std::vector<class Actor*> mActors;
    std::vector<class Actor*> mPendingActors;

    // 貸し出しIDとActorの対応表
    std::unordered_map<ActorID, class Actor*> mLendingMap;

    bool mUpdatingActors;

    void UnloadObjects() override;

    // 次にmLendingMapに登録するIDの値の候補
    ActorID mID;

   public:
    ActorsSystem();
    ~ActorsSystem() override;

    void ProcessInput(const InputState& state);
    void UpdateObjects(float deltatime);

    // Actorが死んでいたり，IDに合致する該当する具体クラスが無ければnullptrを返す．
    // 返り値のActorは必ず所有しない．
    template <typename T>
    T* GetActor(ActorID id) {
        auto iter = mLendingMap.find(id);
        if (iter == mLendingMap.end()) return nullptr;

        // cast
        return dynamic_cast<T*>(iter->second);
    };

    ActorID AddActor(class Actor* actor);
    void RemoveActor(ActorID id);
};

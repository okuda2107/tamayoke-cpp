#pragma once
#include <functional>
#include <vector>

#include "Collision.h"
#include "core/Math.h"

// Collision情報を管理
// Collision群が引数に与えたCollisionが交わっているかどうかや，
// Collision群同士を比較して接触しているモノ同士で処理
class PhysWorld {
    std::unordered_map<std::string, std::vector<class BoxComponent*>> mBoxes;
    std::unordered_map<std::string, std::vector<class SphereComponent*>>
        mSpheres;

    template <typename TShape>
    std::unordered_map<std::string, std::vector<TShape*>>& GetMapArrays();

   public:
    PhysWorld() = default;
    ~PhysWorld() = default;

    // Used to give helpful information about collision results
    struct CollisionInfo {
        // Point of collision
        Vector3 mPoint;
        // Normal at collision
        Vector3 mNormal;
        // Component collided with
        class BoxComponent* mBox;
        // Owning actor of component
        class Actor* mActor;
    };

    // Test a line segment against boxes
    // Returns true if it collides against a box
    // bool SegmentCast(const LineSegment& l, CollisionInfo& outColl);

    // Tests collisions using naive pairwise
    // void TestPairwise(std::function<void(class Actor*, class Actor*)> f);
    // Test collisions using sweep and prune
    // void TestSweepAndPrune(std::function<void(class Actor*, class Actor*)> f);

    // Add/remove box components from world
    void AddBox(const std::string& tag, class BoxComponent* box);
    void RemoveBox(const std::string& tag, class BoxComponent* box);

    void AddSphere(const std::string& tag, class SphereComponent* sphere);
    void RemoveSphere(const std::string& tag, class SphereComponent* sphere);

    // tagに一致する配列が無い場合nullptrを返す．
    template <typename T>
    const std::vector<T*>* GetArray(std::string tag) {
        std::unordered_map<std::string, std::vector<T*>>& mapArray =
            GetMapArrays<T>();
        auto iter = mapArray.find(tag);
        if (iter != mapArray.end()) {
            return &iter->second;
        } else {
            return nullptr;
        }
    }
};

template <>
inline std::unordered_map<std::string, std::vector<class BoxComponent*>>&
PhysWorld::GetMapArrays() {
    return mBoxes;
}

template <>
inline std::unordered_map<std::string, std::vector<class SphereComponent*>>&
PhysWorld::GetMapArrays() {
    return mSpheres;
}

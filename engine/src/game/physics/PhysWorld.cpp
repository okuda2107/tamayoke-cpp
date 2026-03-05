#include "game/physics/PhysWorld.h"

#include <algorithm>

#include "SDL.h"

// bool PhysWorld::SegmentCast(const LineSegment& l, CollisionInfo& outColl) {
//     bool collided = false;
//     // Initialize closestT to infinity, so first
//     // intersection will always update closestT
//     float closestT = Math::Infinity;
//     Vector3 norm;
//     // Test against all boxes
//     for (auto box : mBoxes) {
//         float t;
//         // Does the segment intersect with the box?
//         if (Intersect(l, box->GetWorldBox(), t, norm)) {
//             // Is this closer than previous intersection?
//             if (t < closestT) {
//                 closestT = t;
//                 outColl.mPoint = l.PointOnSegment(t);
//                 outColl.mNormal = norm;
//                 outColl.mBox = box;
//                 outColl.mActor = box->GetOwner();
//                 collided = true;
//             }
//         }
//     }
//     return collided;
// }

// void PhysWorld::TestPairwise(std::function<void(Actor*, Actor*)> f) {
//     // Naive implementation O(n^2)
//     for (size_t i = 0; i < mBoxes.size(); i++) {
//         // Don't need to test vs itself and any previous i values
//         for (size_t j = i + 1; j < mBoxes.size(); j++) {
//             BoxComponent* a = mBoxes[i];
//             BoxComponent* b = mBoxes[j];
//             if (Intersect(a->GetWorldBox(), b->GetWorldBox())) {
//                 // Call supplied function to handle intersection
//                 f(a->GetOwner(), b->GetOwner());
//             }
//         }
//     }
// }

// 接触しているActor同士で何か与えた関数の処理をさせる関数
// void PhysWorld::TestSweepAndPrune(std::function<void(Actor*, Actor*)> f) {
//     // Sort by min.x
//     std::sort(mBoxes.begin(), mBoxes.end(),
//               [](BoxComponent* a, BoxComponent* b) {
//                   return a->GetWorldBox().mMin.x < b->GetWorldBox().mMin.x;
//               });

//     for (size_t i = 0; i < mBoxes.size(); i++) {
//         // Get max.x for current box
//         BoxComponent* a = mBoxes[i];
//         float max = a->GetWorldBox().mMax.x;
//         for (size_t j = i + 1; j < mBoxes.size(); j++) {
//             BoxComponent* b = mBoxes[j];
//             // If AABB[j] min is past the max bounds of AABB[i],
//             // then there aren't any other possible intersections
//             // against AABB[i]
//             if (b->GetWorldBox().mMin.x > max) {
//                 break;
//             } else if (Intersect(a->GetWorldBox(), b->GetWorldBox())) {
//                 f(a->GetOwner(), b->GetOwner());
//             }
//         }
//     }
// }

void PhysWorld::AddBox(const std::string& tag, BoxComponent* box) {
    mBoxes[tag].push_back(box);
}

void PhysWorld::RemoveBox(const std::string& tag, BoxComponent* box) {
    auto mapIter = mBoxes.find(tag);
    if (mapIter == mBoxes.end()) return;
    auto& boxes = mapIter->second;
    auto iter = std::find(boxes.begin(), boxes.end(), box);
    if (iter != boxes.end()) {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, boxes.end() - 1);
        boxes.pop_back();
    }
}

void PhysWorld::AddSphere(const std::string& tag, SphereComponent* sphere) {
    mSpheres[tag].push_back(sphere);
}

void PhysWorld::RemoveSphere(const std::string& tag, SphereComponent* sphere) {
    auto mapIter = mSpheres.find(tag);
    if (mapIter == mSpheres.end()) return;
    auto& spheres = mapIter->second;
    auto iter = std::find(spheres.begin(), spheres.end(), sphere);
    if (iter != spheres.end()) {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, spheres.end() - 1);
        spheres.pop_back();
    }
}

#include "game/physics/Collision.h"

#include <algorithm>
#include <array>

LineSegment::LineSegment(const Vector3& start, const Vector3& end)
    : mStart(start), mEnd(end) {}

Vector3 LineSegment::PointOnSegment(float t) const {
    return (mEnd - mStart) * t + mStart;
}

float LineSegment::MinDistSq(const Vector3& point) const {
    Vector3 ab = mEnd - mStart;
    Vector3 ba = -1.0f * ab;
    Vector3 ac = point - mStart;
    Vector3 bc = point - mEnd;

    // ケース1: CがAの前に突き出ている
    if (Vector3::Dot(ab, ac) < 0.0f) return ac.LengthSq();

    // ケース2: CがBのあとに突き出ている
    if (Vector3::Dot(ba, bc) < 0.0f) return bc.LengthSq();

    // ケース3: 点から線分に垂線を下せる場合
    // Cを線分に射影する． 射影点pを計算
    float scalar = Vector3::Dot(ac, ab) / Vector3::Dot(ab, ab);
    Vector3 p = scalar * ab;

    return (ac - p).LengthSq();
}

float LineSegment::MinDistSq(const LineSegment& s1, const LineSegment& s2) {
    // 2つの直線間のベクトルを2直線のパラメータs, tで表し，それを最小化する
    Vector3 u = s1.mEnd - s1.mStart;
    Vector3 v = s2.mEnd - s2.mStart;
    Vector3 w = s1.mStart - s2.mStart;
    float a = Vector3::Dot(u, u);
    float b = Vector3::Dot(u, v);
    float c = Vector3::Dot(v, v);
    float d = Vector3::Dot(u, w);
    float e = Vector3::Dot(v, w);
    float D = a * c - b * b;  // always >= 0
    float sc, sN, sD = D;     // sc = sN / sD, default sD = D >= 0
    float tc, tN, tD = D;     // tc = tN / tD, default tD = D >= 0

    // compute the line parameters of the two closest points
    if (Math::NearZero(D)) {  // the lines are almost parallel
        sN = 0.0;             // force using point P0 on segment S1
        sD = 1.0;             // to prevent possible division by 0.0 later
        tN = e;
        tD = c;
    } else {  // get the closest points on the infinite lines
        sN = (b * e - c * d);
        tN = (a * e - b * d);
        if (sN < 0.0) {  // sc < 0 => the s=0 edge is visible
            sN = 0.0;
            tN = e;
            tD = c;
        } else if (sN > sD) {  // sc > 1  => the s=1 edge is visible
            sN = sD;
            tN = e + b;
            tD = c;
        }
    }

    if (tN < 0.0) {  // tc < 0 => the t=0 edge is visible
        tN = 0.0;
        // recompute sc for this edge
        if (-d < 0.0)
            sN = 0.0;
        else if (-d > a)
            sN = sD;
        else {
            sN = -d;
            sD = a;
        }
    } else if (tN > tD) {  // tc > 1  => the t=1 edge is visible
        tN = tD;
        // recompute sc for this edge
        if ((-d + b) < 0.0)
            sN = 0;
        else if ((-d + b) > a)
            sN = sD;
        else {
            sN = (-d + b);
            sD = a;
        }
    }
    // finally do the division to get sc and tc
    sc = (Math::NearZero(sN) ? 0.0f : sN / sD);
    tc = (Math::NearZero(tN) ? 0.0f : tN / tD);

    // get the difference of the two closest points
    Vector3 dP = w + (sc * u) - (tc * v);  // =  S1(sc) - S2(tc)

    return dP.LengthSq();  // return the closest distance squared
}

Plane::Plane(const Vector3& a, const Vector3& b, const Vector3& c) {
    Vector3 ab = b - a;
    Vector3 ac = c - a;
    // クロス積と正規化で法線を得る
    mNormal = Vector3::Cross(ab, ac);
    mNormal.Normalize();

    // 原点Oから法線を伸ばして平面との交点をpとすると，三角形Opaはpが直角の三角形になる．
    // つまり求めるのはVector aの法線成分を逆にした長さ
    mD = -Vector3::Dot(a, mNormal);
}

// 戻り値が負の値の時，pointは平面から見て，法線と反対の向きにあり，正の値の時，法線と同じ向きにある．
float Plane::SignedDist(const Vector3& point) const {
    return Vector3::Dot(point, mNormal) - mD;
}

// 球が点を含むかどうかの判定をしている
// 包含判定はある領域の内側にいるのか判定するために使える
bool Sphere::Contains(const Vector3& point) const {
    // 中心と点との距離を求める
    float distSq = (mCenter - point).LengthSq();
    return distSq <= (mRadius * mRadius);
}

AABB::AABB(const Vector3& min, const Vector3& max) : mMin(min), mMax(max) {}

void AABB::UpdateMinMax(const Vector3& point) {
    // Update each component separately
    mMin.x = Math::Min(mMin.x, point.x);
    mMin.y = Math::Min(mMin.y, point.y);
    mMin.z = Math::Min(mMin.z, point.z);

    mMax.x = Math::Max(mMax.x, point.x);
    mMax.y = Math::Max(mMax.y, point.y);
    mMax.z = Math::Max(mMax.z, point.z);
}

void AABB::Rotate(const Quaternion& q) {
    // Construct the 8 points for the corners of the box
    std::array<Vector3, 8> points;
    // Min point is always a corner
    points[0] = mMin;
    // Permutations with 2 min and 1 max
    points[1] = Vector3(mMax.x, mMin.y, mMin.z);
    points[2] = Vector3(mMin.x, mMax.y, mMin.z);
    points[3] = Vector3(mMin.x, mMin.y, mMax.z);
    // Permutations with 2 max and 1 min
    points[4] = Vector3(mMin.x, mMax.y, mMax.z);
    points[5] = Vector3(mMax.x, mMin.y, mMax.z);
    points[6] = Vector3(mMax.x, mMax.y, mMin.z);
    // Max point corner
    points[7] = Vector3(mMax);

    // Rotate first point
    Vector3 p = Vector3::Transform(points[0], q);
    // Reset min/max to first point rotated
    mMin = p;
    mMax = p;
    // Update min/max based on remaining points, rotated
    for (size_t i = 1; i < points.size(); i++) {
        p = Vector3::Transform(points[i], q);
        UpdateMinMax(p);
    }
}

bool AABB::Contains(const Vector3& point) const {
    bool outside = point.x < mMin.x || point.y < mMin.y || point.z < mMin.z ||
                   point.x > mMax.x || point.y > mMax.y || point.z > mMax.z;
    // If none of these are true, the point is inside the box
    return !outside;
}

// AABBの中に入ってたら0を返す
float AABB::MinDistSq(const Vector3& point) const {
    float dx = Math::Max(mMin.x - point.x, 0.0f);
    dx = Math::Max(dx, point.x - mMax.x);
    float dy = Math::Max(mMin.y - point.y, 0.0f);
    dy = Math::Max(dy, point.y - mMax.y);
    float dz = Math::Max(mMin.z - point.z, 0.0f);
    dz = Math::Max(dy, point.z - mMax.z);

    return dx * dx + dy * dy + dz * dz;
}

// OBB

Capsule::Capsule(const Vector3& start, const Vector3& end, float radius)
    : mSegment(LineSegment(start, end)), mRadius(radius) {}

Vector3 Capsule::PointOnSegment(float t) const {
    return mSegment.PointOnSegment(t);
}

bool Capsule::Contains(const Vector3& point) const {
    float distSq = mSegment.MinDistSq(point);
    return distSq <= mRadius;
}

// todo: 未実装
// bool ConvexPolygon::Contains(const Vector2& point) const {
//     ;
//     ;
// }

// バウンディングボリューム間の交差判定
bool Intersect(const Sphere& a, const Sphere& b) {
    float distSq = (a.mCenter - b.mCenter).LengthSq();
    float sumRadii = a.mRadius + b.mRadius;
    return distSq <= (sumRadii * sumRadii);
}

/*
分離軸の定理: 2つの凸オブジェクトA, Bが交差しないならば，A, Bを分離する軸が必ず存在する．
分離する軸とは，A,Bオブジェクトを軸に射影したとき，オブジェクトの範囲が重ならない軸
よって，x, y, zにおいて，それぞれオブジェクトが重なっていないかを判定するだけで良い
*/
bool Intersect(const AABB& a, const AABB& b) {
    bool no = a.mMax.x < b.mMin.x || b.mMax.x < a.mMin.x ||
              a.mMax.y < b.mMin.y || b.mMax.y < a.mMin.y ||
              a.mMax.z < b.mMin.z || b.mMax.z < a.mMin.z;
    return !no;
}

bool Intersect(const Capsule& a, const Capsule& b) {
    float distSq = LineSegment::MinDistSq(a.mSegment, b.mSegment);
    float sumRadii = a.mRadius + b.mRadius;
    return distSq <= (sumRadii * sumRadii);
}

bool Intersect(const Sphere& s, const AABB& box) {
    float distSq = box.MinDistSq(s.mCenter);
    return distSq <= (s.mRadius * s.mRadius);
}

// todo: 未実装
// bool Intersect(const LineSegment& l, const Sphere& s, float& outT) {
//     ;
//     ;
// }

bool Intersect(const LineSegment& l, const Plane& p, float& outT) {
    // First test if there's a solution for t
    float denom = Vector3::Dot(l.mEnd - l.mStart, p.mNormal);
    if (Math::NearZero(denom)) {
        // The only way they intersect is if start
        // is a point on the plane (P dot N) == d
        if (Math::NearZero(Vector3::Dot(l.mStart, p.mNormal) - p.mD)) {
            return true;
        } else {
            return false;
        }
    } else {
        float numer = -Vector3::Dot(l.mStart, p.mNormal) - p.mD;
        outT = numer / denom;
        // Validate t is within bounds of the line segment
        if (outT >= 0.0f && outT <= 1.0f) {
            return true;
        } else {
            return false;
        }
    }
}

// todo: 未実装
// bool Intersect(const LineSegment& l, const AABB& b, float& outT,
//                Vector3& outNorm) {
//     ;
//     ;
// }

// todo: 未実装
// bool SweptSphere(const Sphere& P0, const Sphere& P1, const Sphere& Q0,
//                  const Sphere& Q1, float& t) {
//     ;
//     ;
// }

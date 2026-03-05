#include "core/Random.h"

Vector2 Random::GetVector(const Vector2& min, const Vector2& max) {
    Vector2 r(GetFloat(), GetFloat());
    return min + (max - min) * r;
}

float Random::GetFloat() { return GetFloatRange(0.0f, 1.0f); }

float Random::GetFloatRange(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(sGenerator);  //
}

std::mt19937 Random::sGenerator(std::random_device{}());

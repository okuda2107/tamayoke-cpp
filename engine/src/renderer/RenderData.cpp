#include "renderer/RenderData.h"

ShapeCircle::ShapeCircle(Vector3 center, float radius, Vector3 color,
                         bool filled, int segments)
    : center(center),
      radius(radius),
      color(color),
      filled(filled),
      segments(segments) {
    verts.reserve(segments);
    for (int i = 0; i < segments; i++) {
        float theta = (float)i / segments * Math::TwoPi;
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);

        verts.emplace_back(center + Vector3(x, y, 0.0f));
    }
}

ShapeCircle::ShapeCircle()
    : ShapeCircle(Vector3::Zero, 0, Vector3::Zero, false) {}

void ShapeCircle::Update(Vector3 center, float radius) {
    this->center = center;
    this->radius = radius;

    verts.clear();

    for (int i = 0; i < segments; i++) {
        float theta = (float)i / segments * Math::TwoPi;
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);

        verts.emplace_back(center + Vector3(x, y, 0.0f));
    }
}

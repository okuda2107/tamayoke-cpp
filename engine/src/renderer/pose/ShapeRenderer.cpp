#include "renderer/pose/ShapeRenderer.h"

#include <cstddef>

#include "GL/glew.h"
#include "renderer/Shader.h"

namespace pose {
ShapeRenderer::ShapeRenderer() : mShapeShader(nullptr) {}

ShapeRenderer::~ShapeRenderer() {
    mShapeShader->Unload();
    delete mShapeShader;
}

bool ShapeRenderer::Initialize(float screenWidth, float screenHeight,
                               std::string windowTitle, bool isFullScreen) {
    if (!Renderer::Initialize(screenWidth, screenHeight, windowTitle,
                              isFullScreen))
        return false;

    mShapeShader = new Shader();

    if (!mShapeShader->Load("Shaders/Shape.vert", "Shades/Shape.frag"))
        return false;

    glGenVertexArrays(1, &mShapeVAO);
    glGenBuffers(1, &mShapeVBO);

    return true;
}

void ShapeRenderer::AddLine(const Vector3& a, const Vector3& b,
                            const Vector3& color) {
    mLines.push_back({a, color});
    mLines.push_back({b, color});
}

void ShapeRenderer::AddCircle(const Vector3& center, float r,
                              const Vector3& color) {
    const int segments = 32;

    for (int i = 0; i < segments; i++) {
        float t0 = Math::TwoPi * i / segments;
        float t1 = Math::TwoPi * (i + 1) / segments;

        Vector3 p0(center.x + r * cos(t0), center.y + r * sin(t0), center.z);

        Vector3 p1(center.x + r * cos(t1), center.y + r * sin(t1), center.z);

        AddLine(p0, p1, color);
    }
}

void ShapeRenderer::DrawScene(const RenderData& data) {
    Renderer::DrawScene(data);
    ShapeDraw(data);
}

void ShapeRenderer::ShapeDraw(const RenderData& data) {
    if (mLines.empty()) return;

    glDisable(GL_DEPTH_TEST);

    mShapeShader->SetActive();
    mShapeShader->SetMatrixUniform("uViewProj", data.mView * mProjection);

    glBindVertexArray(mShapeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mShapeVBO);

    glBufferData(GL_ARRAY_BUFFER, mLines.size() * sizeof(ShapeVertex),
                 mLines.data(), GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex),
                          (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex),
                          (void*)offsetof(ShapeVertex, color));

    glDrawArrays(GL_LINES, 0, mLines.size());

    mLines.clear();
}
}  // namespace pose

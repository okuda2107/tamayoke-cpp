#pragma once
#include "../Renderer.h"

namespace pose {
struct ShapeVertex {
    Vector3 position;
    Vector3 color;
};

// 姿勢推定結果を簡単に出力するためにコードを追加する
class ShapeRenderer : public Renderer {
    void ShapeDraw(const RenderData& data);

    std::vector<ShapeVertex> mLines;

    class Shader* mShapeShader;

    GLuint mShapeVAO;
    GLuint mShapeVBO;

   public:
    ShapeRenderer();
    ~ShapeRenderer();

    bool Initialize(float screenWidth, float screenHeight,
                    std::string windowTitle, bool isFullScreen);

    void DrawScene(const RenderData& data) override;

    void AddLine(const Vector3& a, const Vector3& b, const Vector3& color);

    void AddCircle(const Vector3& center, float r, const Vector3& color);
};
}  // namespace pose

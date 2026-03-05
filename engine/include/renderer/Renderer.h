#pragma once
#include <functional>
#include <map>
#include <unordered_map>
#include <vector>

#include "RenderData.h"
#include "SDL.h"
#include "base/RendererBase.h"
#include "core/Math.h"

// 将来的に描画設定を個別に設定できるようにする
struct RenderConfig {
    bool mDepthTest;
    bool mDepthMask;
    bool mBlend;
    bool mCullFaceBack;
    bool mSortByCamera;

    static RenderConfig Dome();
    static RenderConfig Opaque();
    static RenderConfig Translucent();
};

class Renderer : public RendererBase<RenderData> {
   private:
    class Shader* mSpriteShader;
    class VertexArray* mSpriteVerts;

    static std::unordered_map<RenderConfigID, RenderConfig> mMeshConfigs;

    Matrix4 mProjection;

    float mScreenWidth;
    float mScreenHeight;

    SDL_Window* mWindow;
    SDL_GLContext mContext;

    void Shutdown();

    bool LoadShaders();
    void CreateSpriteVerts();
    void SetLightUniforms(class Shader* shader, const struct RenderData& data);
    // これは本来Shader.hファイルに書くべき？ <-
    // 3D表示するときにしか使わんからRendererファイルでいいかも
    // このファイルに新しく光源の配列作っても良し
    // 光の計算はシェーダーにとっては必要不可欠ではない

    // meshを描画する補助関数
    void MeshDraw(const std::vector<MeshComponent*>& meshComps,
                  const struct RenderData& view);

    // 与えられた描画設定を適用する / 解除する
    void ApplyConfig(const RenderConfig& id);
    void ResetConfig();

   public:
    Renderer();
    ~Renderer();
    // レンダラーの初期化処理と終了処理
    bool Initialize(float screenWidth, float screenHeight,
                    std::string windowTitle, bool isFullScreen);

    // フレームの描画
    void Draw(const struct RenderData& data);

    // SDL_Windowは不完全型
    class SDL_Window* GetWindow() const { return mWindow; }

    // order順に比較できるhashを返す
    // ConfigID GetConfigID(const RenderConfig& config);
};

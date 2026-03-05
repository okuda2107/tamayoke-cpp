#include "renderer/Renderer.h"

#include <algorithm>

#include "GL/glew.h"
// #include "UI/UIScreen.h"
#include "game/UI/Font.h"
#include "game/UI/UIScreen.h"
#include "game/object/Actor.h"
#include "renderer/Mesh.h"
#include "renderer/MeshComponent.h"
#include "renderer/RenderData.h"
#include "renderer/Shader.h"
#include "renderer/SkydomeComponent.h"
#include "renderer/SpriteComponent.h"
#include "renderer/Texture.h"
#include "renderer/VertexArray.h"

RenderConfig RenderConfig::Dome() {
    RenderConfig config;
    config.mDepthTest = true;
    config.mDepthMask = true;
    config.mBlend = false;
    config.mCullFaceBack = true;
    config.mSortByCamera = false;
    return config;
}

RenderConfig RenderConfig::Opaque() {
    RenderConfig config;
    config.mDepthTest = true;
    config.mDepthMask = true;
    config.mBlend = false;
    config.mCullFaceBack = false;
    config.mSortByCamera = false;
    return config;
}

RenderConfig RenderConfig::Translucent() {
    RenderConfig config;
    config.mDepthTest = true;
    config.mDepthMask = false;
    config.mBlend = true;
    config.mCullFaceBack = false;
    config.mSortByCamera = true;
    return config;
}

std::unordered_map<RenderConfigID, RenderConfig> Renderer::mMeshConfigs = {
    {RenderConfigID::Dome, RenderConfig::Dome()},
    {RenderConfigID::Opaque, RenderConfig::Opaque()},
    {RenderConfigID::Translucent, RenderConfig::Translucent()}};

Renderer::Renderer()
    : mContext(nullptr),
      mScreenHeight(0.0f),
      mScreenWidth(0.0f),
      mSpriteVerts(nullptr),
      mSpriteShader(nullptr),
      mWindow(nullptr) {}

Renderer::~Renderer() { Shutdown(); }

bool Renderer::Initialize(float screenWidth, float screenHeight,
                          std::string windowTitle, bool isFullScreen) {
    mScreenWidth = screenWidth;
    mScreenHeight = screenHeight;

    if (int sdlResult = SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        SDL_Log("Failed to Initialize SDL video:%s", SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    // depthバッファの指定 各ピクセルの奥行きの情報
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // ダブルバッファリング有効
    // 裏に描画され，swapで描画しきった画面を入れ替える
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    Uint32 flags = isFullScreen
                       ? SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP
                       : SDL_WINDOW_OPENGL;

    mWindow = SDL_CreateWindow(windowTitle.c_str(), 100, 100,
                               static_cast<int>(mScreenWidth),
                               static_cast<int>(mScreenHeight), flags);
    if (!mWindow) {
        SDL_Log("Faled to Create Window:%s", SDL_GetError());
        return false;
    }

    // OpenGLのコンテキスト (リソースの管理や実行を行う環境) の作成
    mContext = SDL_GL_CreateContext(mWindow);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        SDL_Log("Failed to Initialize GLEW");
        return false;
    }
    glGetError();

    if (!LoadShaders()) {
        SDL_Log("Failed to Load Shaders");
        return false;
    }

    CreateSpriteVerts();

    return true;
}

void Renderer::Shutdown() {
    delete mSpriteVerts;
    mSpriteShader->Unload();
    delete mSpriteShader;
    SDL_GL_DeleteContext(mContext);
    SDL_DestroyWindow(mWindow);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void Renderer::Draw(const RenderData& data) {
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 背景描画
    // todo: 複数の背景オブジェクトに対応？
    if (data.mSkydome != nullptr) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);  // 深度書き込み禁止

        // 背景用のビュー行列 (平行移動成分を除去)
        Matrix4 viewNoTrans = data.mView;
        viewNoTrans.mat[3][0] = 0.0f;  // x平行移動消去
        viewNoTrans.mat[3][1] = 0.0f;  // y平行移動消去
        viewNoTrans.mat[3][2] = 0.0f;  // z平行移動消去

        Matrix4 viewProj = viewNoTrans * mProjection;
        data.mSkydome->Draw(viewProj);

        glDepthMask(GL_TRUE);
        glDisable(GL_CULL_FACE);
    }

    // メッシュ描画
    for (auto mc : data.mMeshComps) {
        auto config = mMeshConfigs.at(mc.first);
        ApplyConfig(config);

        if (config.mSortByCamera) {
            // カメラから遠い順に描画するための描画
            // この処理はRenderDBの責務です．
            // と思ったけど，この処理は毎秒行われるべきなので，rendererがしてもいいのかも
            auto comps = mc.second;
            Matrix4 invView = data.mView;
            invView.Invert();
            Vector3 cameraPos = invView.GetTranslation();
            // 半透明相当の描画設定 → 遠い順にソート
            std::sort(
                comps.begin(), comps.end(),
                [cameraPos](MeshComponent* a, MeshComponent* b) {
                    float distA =
                        (a->GetOwner()->GetPosition() - cameraPos).LengthSq();
                    float distB =
                        (b->GetOwner()->GetPosition() - cameraPos).LengthSq();
                    return distA > distB;  // 遠い順
                });
            MeshDraw(comps, data);

        } else {  // カメラから遠い順に描画する必要がないとき
            MeshDraw(mc.second, data);
        }

        ResetConfig();
    }

    // スプライト描画
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

    // Set shader/vao as active
    mSpriteShader->SetActive();
    mSpriteVerts->SetActive();
    for (auto sprite : data.mSprites) {
        sprite->Draw(mSpriteShader);
    }

    // Draw UI
    if (data.mUI) {
        for (auto ui : *data.mUI) {
            ui->Draw(mSpriteShader);
        }
    }

    SDL_GL_SwapWindow(mWindow);
}

void Renderer::MeshDraw(const std::vector<MeshComponent*>& meshComps,
                        const RenderData& data) {
    Shader* previousShader = nullptr;
    for (auto mc : meshComps) {
        if (mc->GetShader() != previousShader) {
            mc->GetShader()->SetActive();
            mc->GetShader()->SetMatrixUniform("uViewProj",
                                              data.mView * mProjection);
            SetLightUniforms(mc->GetShader(), data);
        }
        mc->Draw();
    }
}

// OpenGLの状態をconfigに基づいて適用
void Renderer::ApplyConfig(const RenderConfig& config) {
    if (config.mBlend)
        glEnable(GL_BLEND);
    else if (!config.mBlend)
        glDisable(GL_BLEND);

    if (config.mCullFaceBack) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }

    if (config.mDepthMask)
        glDepthMask(GL_TRUE);
    else if (!config.mDepthMask)
        glDepthMask(GL_FALSE);

    if (config.mDepthTest)
        glEnable(GL_DEPTH_TEST);
    else if (!config.mDepthTest)
        glDisable(GL_DEPTH_TEST);
}

void Renderer::ResetConfig() {
    // todo: 状態をリセットすることを検討
    glDisable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);
}

bool Renderer::LoadShaders() {
    mSpriteShader = new Shader();

    if (!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag")) {
        return false;
    }

    mSpriteShader->SetActive();

    // ビュー射影行列の情報を流し込む
    Matrix4 viewProj =
        Matrix4::CreateSimpleViewProj(mScreenWidth, mScreenHeight);
    mSpriteShader->SetMatrixUniform("uViewProj", viewProj);

    // Set the view-projection matrix
    mProjection = Matrix4::CreatePerspectiveFOV(
        Math::ToRadians(70.0f), mScreenWidth, mScreenHeight, 25.0f, 10000.0f);

    return true;
}

void Renderer::CreateSpriteVerts() {
    float vertexBuffer[] = {
        -0.5f, 0.5f,  0.f, 0.f, 0.f, 0.0f, 0.f, 0.f,  // top left
        0.5f,  0.5f,  0.f, 0.f, 0.f, 0.0f, 1.f, 0.f,  // top right
        0.5f,  -0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f,  // bottom right
        -0.5f, -0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f   // bottom left
    };

    unsigned int indexBuffer[] = {0, 1, 2, 2, 3, 0};

    mSpriteVerts = new VertexArray(vertexBuffer, 4, indexBuffer, 6);
}

//ShaderファイルはGLSLへの架け橋の役目，光のセットアップを書いてしまうとそれが崩れる
void Renderer::SetLightUniforms(Shader* shader, const RenderData& data) {
    // Camera position is from inverted view
    Matrix4 invView = data.mView;
    invView.Invert();
    shader->SetVectorUniform("uCameraPos", invView.GetTranslation());
    // Ambient light
    shader->SetVectorUniform("uAmbientLight", data.mAmbientLight);
    // Directional light
    shader->SetVectorUniform("uDirLight.mDirection", data.mDirLight.mDirection);
    shader->SetVectorUniform("uDirLight.mDiffuseColor",
                             data.mDirLight.mDiffuseColor);
    shader->SetVectorUniform("uDirLight.mSpecColor", data.mDirLight.mSpecColor);
}

/*
現在の実装では，configの項目が少ないため，項目で分岐させて列挙体のTypeIDで管理するので十分
メリット: どのIDがどの設定が分かるため，デバッグが容易
デメリット: 設定項目が多くなると，実装が煩雑．スケールしない．
設定項目が多くなってきたら，設定項目をハッシュ値に変換する実装に切り替える
メリット: 設定項目が多くても，実装がシンプル．
デメリット: ハッシュ値に変換すると，元の設定内容が分からないため，デバッグに工夫が必要．
ハッシュ値を出力するようにする場合，描画順序を考慮させることが難しい．そのため上位ピットのいくつかは描画順序を表す数字にして，mapの描画順序を表す
*/
// ConfigID Renderer::GetConfigID(const RenderConfig& config) {
//     ConfigID id;
//     if (config.mCullFaceBack) {
//         id = ConfigID::Dome;
//     } else if (config.mBlend && !config.mDepthMask && config.mDepthTest) {
//         id = ConfigID::Translucent;
//     } else {
//         id = ConfigID::Opaque;
//     }
//     mMeshConfigs.emplace(id, config);
//     return id;
// }

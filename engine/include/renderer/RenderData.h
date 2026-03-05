#pragma once
#include <map>
#include <vector>

#include "base/RenderDataBase.h"
#include "core/Math.h"

struct DirectionalLight {
    // 光の方向
    Vector3 mDirection;
    // 拡散反射光
    Vector3 mDiffuseColor;
    // 鏡面反射光
    Vector3 mSpecColor;
};

// 将来的にハッシュ値になる可能性
// 現在はここで順序を設定
enum RenderConfigID {
    Dome = 0,
    Opaque,
    Translucent,
    NUM_CONFIG_ID,
};

struct RenderData : RenderDataBase {
    Matrix4 mView;

    std::vector<class SpriteComponent*> mSprites;
    std::map<RenderConfigID, std::vector<class MeshComponent*>> mMeshComps;

    class SkydomeComponent* mSkydome;

    // 表示するUIを渡す
    // constポインタにすると，ポインタの先の内容を変更はできない．
    // 変数がconstなわけでは無いので，mUIへポインタを再代入はできる．
    const std::vector<class UIScreen*>* mUI;

    // Lighting data
    Vector3 mAmbientLight;
    DirectionalLight mDirLight;

    RenderData() : mSkydome(nullptr), mUI(nullptr) {};
};

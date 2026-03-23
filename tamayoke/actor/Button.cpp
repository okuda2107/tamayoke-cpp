#include "Button.h"

#include <sstream>

#include "component/ButtonAnimComp.h"
#include "game/physics/BoxComponent.h"
#include "game/physics/PhysWorld.h"
#include "game/physics/SphereComponent.h"
#include "renderer/RenderDB.h"
#include "renderer/Texture.h"

Button::Button(class ActorsSystem* system, ButtonDeps& deps)
    : Actor(system, deps), physSystem(deps.physics) {
    // 位置調整
    float x = 0;
    float y = -200;
    float scale = 1.5;
    SetPosition(Vector3(x, y, 0));
    SetScale(scale);

    // button
    mAnimSpriteComp = new ButtonAnimComp(this, &deps.renderDB);
    for (int i = 0; i < 69; i++) {
        std::stringstream ss;
        ss << "Assets/start/start" << i << ".png";
        mAnimSpriteComp->SetTexture(i, deps.renderDB.GetTexture(ss.str()));
    }
    mAnimSpriteComp->SetFPS(69 / 2);
    mAnimSpriteComp->SetEnable(false);

    // 当たり判定
    mBoxComp =
        new BoxComponent(this, "button", CollisionCompDeps(deps.physics));
    Texture* tex = deps.renderDB.GetTexture("Assets/start/start0.png");
    float boxWitdh = tex->GetWidth();
    float boxHeight = tex->GetHeight();
    // テクスチャの幅と高さをそのまま使うと，表示されているboxと誤差が出るので，
    // 一定の倍率をかける
    Vector3 minPoint = Vector3(boxWitdh, boxHeight, 0) * (-0.5 / 6);
    Vector3 maxPoint = Vector3(boxWitdh, boxHeight, 0) * (0.5 / 6);
    auto box = AABB(minPoint, maxPoint);
    mBoxComp->SetObjectBox(box);
}

void Button::UpdateActor(float deltatime) {
    auto vec = physSystem.GetArray<SphereComponent>("hand");
    // とりあえず
    if (!vec) return;
    assert(vec);
    bool isTouch = false;
    for (auto c : *vec) {
        if (Intersect(c->mSphere, mBoxComp->GetWorldBox())) {
            isTouch = true;
            break;
        }
    }

    bool isPrevTouch = mAnimSpriteComp->GetEnableFlag();

    if (isTouch && isPrevTouch) {
        isFlag = mAnimSpriteComp->GetTime() * 69 / 2 > 68;
    } else if (isTouch && !isPrevTouch) {
        mAnimSpriteComp->SetEnable(true);
    } else {
        mAnimSpriteComp->SetEnable(false);
    }
}

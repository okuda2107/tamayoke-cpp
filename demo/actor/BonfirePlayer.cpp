#include "BonfirePlayer.h"

#include "../UI/AddWoodUI.h"
#include "../UI/AlreadyHaveWoodUI.h"
#include "../UI/HaveWoodUI.h"
#include "../Utility/Coroutine.h"
#include "Bonfire.h"
#include "Wood.h"
#include "core/Math.h"
#include "game/physics/PhysWorld.h"
#include "game/physics/SphereComponent.h"
#include "input/InputSystem.h"

BonfirePlayer::BonfirePlayer(class ActorsSystem* system, BonfirePlayerDeps deps)
    : FPSActor(system, deps),
      mIsAnimLookUp(false),
      mIsAnimLookDown(false),
      mLookUpEndTime(-1.0f),
      mLookDownEndTime(-1.0f),
      mHasWood(false),
      mWoodUIID(-1),
      mBonfireUIID(-1),
      mFieldMin(nullptr),
      mFieldMax(nullptr),
      mActorsSystem(*system),
      mPhysWorld(deps.physWorld),
      mSphereComp(nullptr),
      mUISystem(deps.uiSystem),
      mRenderDB(deps.renderDB),
      mStateManager(deps.stateManager) {
    mCoroutines = new Coroutine();
    mSphereComp =
        new SphereComponent(this, "player", CollisionCompDeps(mPhysWorld));
    // todo: Sphereの半径を調整
    Sphere sphere{};
    sphere.mCenter = GetPosition();
    sphere.mRadius = 50.0f;
    mSphereComp->mSphere = sphere;
}

BonfirePlayer::~BonfirePlayer() {
    delete mCoroutines;

    auto woodUI = mUISystem.GetUI<HaveWoodUI>(mWoodUIID);
    if (woodUI) woodUI->Close();
    auto bonfireUI = mUISystem.GetUI<AddWoodUI>(mBonfireUIID);
    if (bonfireUI) bonfireUI->Close();
}

void BonfirePlayer::ActorInput(const InputState& state) {
    // アニメーション中は操作を受け付けない
    if (mIsAnimLookUp || mIsAnimLookDown) return;
    FPSActor::ActorInput(state);

    // 木が近くにあるとき，Eキーを押すと拾う
    // 既に持っている場合は拾えない
    auto woods =
        mPhysWorld.GetArray<SphereComponent>(Wood::SWoodPhysTag.data());
    if (woods) {
        for (auto wood : *woods) {
            if (Intersect(mSphereComp->mSphere, wood->mSphere) &&
                state.Keyboard.GetKeyState(SDL_SCANCODE_E)) {
                if (!mHasWood) {
                    mHasWood = true;
                    wood->GetOwner()->SetState(Actor::State::EDead);
                    break;
                } else {
                    new AlreadyHaveWoodUI(
                        &mUISystem, BonfireUIDeps(mRenderDB, mStateManager));
                }
            }
        }
    }

    // Bonfireが近くにある時，Eキーを押すと木をくべられる
    auto bonfires =
        mPhysWorld.GetArray<SphereComponent>(Bonfire::SBonfirePhysTag.data());
    if (bonfires) {
        for (auto bonfire : *bonfires) {
            if (Intersect(mSphereComp->mSphere, bonfire->mSphere) && mHasWood &&
                state.Keyboard.GetKeyState(SDL_SCANCODE_E)) {
                mHasWood = false;
                auto bonfireInstance =
                    mActorsSystem.GetActor<Bonfire>(mBonfireID);
                if (bonfireInstance) bonfireInstance->AddWood();
            }
        }
    }
}

void BonfirePlayer::UpdateActor(float deltatime) {
    mCoroutines->Update(deltatime);
    if (mIsAnimLookUp) {
        if (mLookUpEndTime < 0)
            mLookUpEndTime = mCoroutines->AddCoroutine(
                2.0, [this](float t) { this->PitchUp(t); });
        if (mCoroutines->CheckCoroutine(mLookUpEndTime)) {
            mIsAnimLookUp = false;
            mLookUpEndTime = -1.0f;
        }
    } else if (mIsAnimLookDown) {
        if (mLookDownEndTime < 0)
            mLookDownEndTime = mCoroutines->AddCoroutine(
                2.0, [this](float t) { this->PitchDown(t); });
        if (mCoroutines->CheckCoroutine(mLookDownEndTime)) {
            mIsAnimLookDown = false;
            mLookDownEndTime = -1.0f;
        }
    }
    FPSActor::UpdateActor(deltatime);

    // 位置の制限
    if (mFieldMin && mFieldMax) {
        Vector3 pos = GetPosition();
        pos.x = Math::Clamp(pos.x, mFieldMin->x, mFieldMax->x);
        pos.y = Math::Clamp(pos.y, mFieldMin->y, mFieldMax->y);
        SetPosition(pos);
    }

    // 木が近くにある時，UIを出す制御
    auto woods =
        mPhysWorld.GetArray<SphereComponent>(Wood::SWoodPhysTag.data());
    if (woods) {
        bool flag = false;
        for (auto wood : *woods) {
            if (Intersect(mSphereComp->mSphere, wood->mSphere)) {
                flag = true;
                break;
            }
        }
        // mWoodUIIDは実質前フレームまでの衝突情報となっている
        if (mWoodUIID == -1 && flag && !mHasWood) {
            auto woodUI = new HaveWoodUI(
                &mUISystem, BonfireUIDeps(mRenderDB, mStateManager));
            mWoodUIID = woodUI->GetID();
        } else if (mWoodUIID != -1 && !flag) {
            auto woodUI = mUISystem.GetUI<HaveWoodUI>(mWoodUIID);
            if (woodUI) woodUI->Close();
            mWoodUIID = -1;
        }
    }

    // Bonfireが近くにある時，UIを出す制御
    auto bonfires =
        mPhysWorld.GetArray<SphereComponent>(Bonfire::SBonfirePhysTag.data());
    if (bonfires) {
        bool flag = false;
        for (auto bonfire : *bonfires) {
            if (Intersect(mSphereComp->mSphere, bonfire->mSphere)) {
                flag = true;
                break;
            }
        }
        if (flag && mBonfireUIID == -1 && mHasWood) {
            auto bonfireUI = new AddWoodUI(
                &mUISystem, BonfireUIDeps(mRenderDB, mStateManager));
            mBonfireUIID = bonfireUI->GetID();
        } else if (mBonfireUIID != -1 && (!flag || !mHasWood)) {
            auto bonfireUI = mUISystem.GetUI<AddWoodUI>(mBonfireUIID);
            if (bonfireUI) bonfireUI->Close();
            mBonfireUIID = -1;
        }
    }
}

float BonfirePlayer::Ease(float t) { return 0.5f * (1 - cosf(Math::Pi * t)); }

// カメラの視線を下げる
void BonfirePlayer::PitchDown(float lerp) {
    float t = Ease(lerp);
    SetPitchAngular(Math::Lerp(-Math::Pi / 4, 0, t));
}

// カメラの視線を上げる
void BonfirePlayer::PitchUp(float lerp) {
    float t = Ease(lerp);
    SetPitchAngular(Math::Lerp(0, -Math::Pi / 4, t));
}

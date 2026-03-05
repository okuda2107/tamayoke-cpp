#pragma once
#include "game/UI/UIScreen.h"
#include "game/camera/FPSActor.h"

struct BonfirePlayerDeps : FPSActorDeps {
    class PhysWorld& physWorld;
    class UISystem& uiSystem;
    class StateManager& stateManager;

    BonfirePlayerDeps(class RenderDB& renderDB, class AudioSystem& audioSystem,
                      class PhysWorld& physWorld, class UISystem& uiSystem,
                      class StateManager& state)
        : FPSActorDeps(renderDB, audioSystem),
          physWorld(physWorld),
          uiSystem(uiSystem),
          stateManager(state) {}
};

// フィールドによる位置の制限とアニメーション機能を付けたクラス
// 初期化処理として，フィールドの範囲をセットする必要がある．
class BonfirePlayer : public FPSActor {
    bool mIsAnimLookUp;
    bool mIsAnimLookDown;
    // 0未満なら初期値
    float mLookUpEndTime;
    float mLookDownEndTime;

    // 木生成器
    ActorID mGeneratorID;

    //UI
    // 木に関するUIを保持
    UIID mWoodUIID;
    // たき火に関するUIを保持
    UIID mBonfireUIID;

    // 木を持っているか
    bool mHasWood;

    ActorID mBonfireID;

    // コルーチンハンドラ
    class Coroutine* mCoroutines;

    // 視線を上げる / 下げる アニメーション
    static float Ease(float t);
    void PitchUp(float lerp);
    void PitchDown(float lerp);

    Vector2* mFieldMin;
    Vector2* mFieldMax;

    class ActorsSystem& mActorsSystem;
    class PhysWorld& mPhysWorld;

    class SphereComponent* mSphereComp;

    class UISystem& mUISystem;
    class RenderDB& mRenderDB;
    class StateManager& mStateManager;

   public:
    BonfirePlayer(class ActorsSystem* system, BonfirePlayerDeps deps);
    ~BonfirePlayer();

    void ActorInput(const InputState& state) override;
    void UpdateActor(float deltatime) override;

    void SetAnimLookUp() { mIsAnimLookUp = true; }
    void SetAnimLookDown() { mIsAnimLookDown = true; }
    bool GetAnimLookUp() { return mIsAnimLookUp; }
    bool GetAnimLookDown() { return mIsAnimLookDown; }

    void SetBonfireID(ActorID id) { mBonfireID = id; }

    // Playerが動ける範囲を指定
    void SetFieldRange(Vector2& fieldMin, Vector2& fieldMax) {
        mFieldMin = &fieldMin;
        mFieldMax = &fieldMax;
    }
};

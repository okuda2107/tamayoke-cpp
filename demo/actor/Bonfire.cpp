#include "Bonfire.h"

#include "BonfirePlayer.h"
#include "game/audio/AudioComponent.h"
#include "game/object/ActorsSystem.h"
#include "game/physics/SphereComponent.h"
#include "renderer/AnimMeshComponent.h"
#include "renderer/Mesh.h"
#include "renderer/RenderDB.h"

Bonfire::Bonfire(ActorsSystem* system, BonfireDeps& deps)
    : Actor(system, deps),
      mLimit(0.0f),
      cMaxLimit(30.0f),
      mIsRunning(false),
      mFinished(false),
      mAudioComp(nullptr),
      mEventTag("takibi"),
      mPlayerID(0),
      mActorsSystem(*system) {
    SetPosition(Vector3(100, 50, -50));
    SetScale(100.0);
    AnimMeshComponent* mc = new AnimMeshComponent(this, &deps.renderDB,
                                                  RenderConfigID::Translucent);
    mc->SetMesh(deps.renderDB.GetMesh("Assets/Bonfire.gpmesh"));
    mc->SetShader(deps.renderDB.GetShader("Shaders/Anim"));

    mAudioComp = new AudioComponent(this, deps.audioSystem);
    if (!mAudioComp->RegisterEvent(mEventTag))
        SDL_Log("Failed to register event");
    auto event = mAudioComp->GetEvent(mEventTag);
    event.Restart();

    mLimit = cMaxLimit;

    SphereComponent* sphereComp = new SphereComponent(
        this, SBonfirePhysTag.data(), CollisionCompDeps(deps.physWorld));
    // todo: Sphereの半径を調整
    Sphere sphere{};
    sphere.mCenter = GetPosition();
    sphere.mRadius = 50.0f;
    sphereComp->mSphere = sphere;
}

// todo: 時間で消える処理を書く
void Bonfire::UpdateActor(float deltatime) {
    if (!mIsRunning) return;
    // 時間更新
    mLimit -= deltatime;
    if (mLimit <= 0) {
        mFinished = true;
        return;
    }

    SetScale(mLimit / cMaxLimit * 100.0f);
    // 音声処理
    auto event = mAudioComp->GetEvent(mEventTag);
    event.SetVolume(mLimit / cMaxLimit);
}

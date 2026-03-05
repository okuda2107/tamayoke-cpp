#pragma once
#include "game/audio/SoundHandler.h"
#include "game/object/Actor.h"

struct BonfireDeps : ActorDeps {
    class RenderDB& renderDB;
    class AudioSystem& audioSystem;
    class PhysWorld& physWorld;

    BonfireDeps(class RenderDB& db, class AudioSystem& audio,
                class PhysWorld& phys)
        : ActorDeps(), renderDB(db), audioSystem(audio), physWorld(phys) {}
};

class Bonfire : public Actor {
    float mLimit;
    // ロードした瞬間から動いてしまう．
    // Title画面では更新処理はしてほしくないので，更新処理をするかしないかのフラグ
    bool mIsRunning;
    bool mFinished;

    // 簡単のため，BonfirePlayerの位置をこちらで把握
    ActorID mPlayerID;

    class AudioComponent* mAudioComp;
    std::string mEventTag;

    const float cMaxLimit;

    class ActorsSystem& mActorsSystem;

   public:
    Bonfire(class ActorsSystem* system, BonfireDeps& deps);

    void UpdateActor(float deltatime) override;

    void SetRunning() { mIsRunning = true; }
    void AddWood() {
        mLimit += 20;
        if (mLimit > cMaxLimit + 30) mLimit = cMaxLimit + 30;
    }
    void SetPlayer(ActorID id) { mPlayerID = id; }

    bool GetFinished() { return mFinished; }

    static constexpr std::string_view SBonfirePhysTag = "bonfire";
};

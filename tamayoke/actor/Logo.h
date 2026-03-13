#pragma once
#include "game/object/Actor.h"

struct LogoDeps : ActorDeps {
    class RenderDB& renderDB;
    class AudioSystem& audioSystem;

    LogoDeps(class RenderDB& db, class AudioSystem& audio)
        : ActorDeps(), renderDB(db), audioSystem(audio) {}
};

class Logo : public Actor {
    class SpriteComponent* mSpriteComp;
    class AudioComponent* mAudioComp;

    static constexpr std::string mEventTag = "title";

   public:
    Logo(class ActorsSystem* system, LogoDeps& _);
};

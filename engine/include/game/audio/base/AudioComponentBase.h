#pragma once
#include "game/object/Component.h"

template <typename AudioSystem>
class AudioComponentBase : public Component {
   protected:
    AudioSystem* mSystem;

   public:
    AudioComponentBase(class Actor* owner, AudioSystem* system,
                       int updateOrder = 200)
        : Component(owner, updateOrder), mSystem(system) {}

    virtual ~AudioComponentBase() {}

    AudioSystem* GetSystem() { return mSystem; }
};

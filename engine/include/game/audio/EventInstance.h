#pragma once
#include <string>

#include "AL/al.h"

// イベントの実体
// このクラスはEventクラスで管理される．
class EventInstance {
    class Event* mEvent;
    ALuint mSource;

   public:
    EventInstance(Event* event);
    ~EventInstance();

    class Event* GetEvent() { return mEvent; }
    ALuint GetSource() { return mSource; }
    ALint GetState() {
        ALint state;
        alGetSourcei(mSource, AL_SOURCE_STATE, &state);
        return state;
    }
};

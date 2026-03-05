#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include "AL/alut.h"
#include "document.h"

// イベント，サンプルデータ，ストリーミングデータの保持を最適化
class Bank {
    std::unordered_map<std::string, class Event*> mEvents;
    std::unordered_map<std::string, ALuint> mSounds;

    bool LoadVersion1(rapidjson::Document& doc);

   public:
    Bank();
    ~Bank();

    bool Load(const std::string& fileName);
    bool Unload();

    std::unordered_map<std::string, class Event*>& GetEvents() {
        return mEvents;
    }

    ALuint GetSound(const std::string soundID) {
        auto iter = mSounds.find(soundID);
        if (iter != mSounds.end()) {
            return iter->second;
        } else {
            return AL_NONE;
        }
    }
};

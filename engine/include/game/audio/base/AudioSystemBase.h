#pragma once
#include <unordered_map>

#include "core/Math.h"

/*
    AudioComponent: Actorで音声情報を扱うためのComponent
    SoundHandler: 音声を操作するためのhandler
*/
template <typename SoundHandler>
class AudioSystemBase {
   protected:
    virtual void Shutdown() = 0;

   public:
    AudioSystemBase() = default;
    virtual ~AudioSystemBase() = default;

    virtual bool Initialize() = 0;

    virtual void LoadBank(const std::string& name) = 0;
    virtual void UnloadBank(const std::string& name) = 0;

    virtual void Update(float deltaTime) = 0;

    virtual void SetListener(const Matrix4& viewMatrix) = 0;

    virtual SoundHandler PlayEvent(const std::string& name) = 0;
};

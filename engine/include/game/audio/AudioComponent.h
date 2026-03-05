#pragma once
#include <unordered_map>
#include <vector>

#include "SoundHandler.h"
#include "base/AudioComponentBase.h"

struct AudioCompDeps {
    class AudioSystem& system;

    AudioCompDeps(class AudioSystem& system) : system(system) {}
};

class AudioComponent : public AudioComponentBase<class AudioSystem> {
    // ハンドラはAudioSystemでnewされたものを受け取るが，deleteはこちらの責務
    std::unordered_map<std::string, class SoundHandler> mEvents2D;
    std::unordered_map<std::string, class SoundHandler> mEvents3D;

   public:
    AudioComponent(Actor* owner, struct AudioCompDeps acd);
    ~AudioComponent();

    void Update(float deltaTime) override;
    void OnUpdateWorldTransform() override;

    SoundHandler GetEvent(const std::string& name);

    bool RegisterEvent(const std::string& name);
    void StopAllEvents();

    TypeID GetType() const { return TypeID::TAudioComponent; }
};

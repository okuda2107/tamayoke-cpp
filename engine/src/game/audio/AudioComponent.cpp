#include "game/audio/AudioComponent.h"

#include "game/audio/AudioSystem.h"
#include "game/audio/SoundHandler.h"
#include "game/audio/base/AudioComponentBase.h"
#include "game/object/Actor.h"

AudioComponent::AudioComponent(Actor* owner, AudioCompDeps acd)
    : AudioComponentBase(owner, &acd.system) {}

AudioComponent::~AudioComponent() { StopAllEvents(); }

void AudioComponent::Update(float deltaTime) {
    Component::Update(deltaTime);

    // Remove invalid 2D events
    auto iter = mEvents2D.begin();
    while (iter != mEvents2D.end()) {
        if (!iter->second.IsValid()) {
            iter = mEvents2D.erase(iter);
        } else {
            ++iter;
        }
    }

    // Remove invalid 3D events
    iter = mEvents3D.begin();
    while (iter != mEvents3D.end()) {
        if (!iter->second.IsValid()) {
            iter = mEvents3D.erase(iter);
        } else {
            ++iter;
        }
    }
}

void AudioComponent::OnUpdateWorldTransform() {
    // Update 3D event's world transforms
    Matrix4 world = mOwner->GetWorldTransform();
    for (auto event : mEvents3D) {
        if (event.second.IsValid()) {
            event.second.Set3DAttributes(world);
        }
    }
}

//
SoundHandler AudioComponent::GetEvent(const std::string& name) {
    auto iter3D = mEvents3D.find(name);
    if (iter3D != mEvents3D.end() && iter3D->second.IsValid())
        return iter3D->second;
    auto iter2D = mEvents2D.find(name);
    if (iter2D != mEvents2D.end() && iter2D->second.IsValid())
        return iter2D->second;
    return SoundHandler(mSystem, 0);
}

// AudioSystemに要請して，イベントを再生する準備をする．
bool AudioComponent::RegisterEvent(const std::string& name) {
    // ここでSoundHandlerがnewされる．
    SoundHandler e = mSystem->PlayEvent(name);
    if (!e.IsValid()) return false;
    // Is this 2D or 3D?
    if (e.Is3D()) {
        mEvents3D.emplace(name, e);
        // Set initial 3D attributes
        e.Set3DAttributes(mOwner->GetWorldTransform());
    } else {
        mEvents2D.emplace(name, e);
    }

    return true;
}

void AudioComponent::StopAllEvents() {
    // Stop all sounds
    for (auto& e : mEvents2D) {
        e.second.Stop();
    }
    for (auto& e : mEvents3D) {
        e.second.Stop();
    }
    mEvents2D.clear();
    mEvents3D.clear();
}

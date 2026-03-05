#include "game/object/Actor.h"

#include <algorithm>

#include "game/object/ActorsSystem.h"
#include "game/object/Component.h"
// #include "core/LevelLoader.h"

Actor::Actor(ActorsSystem* system, ActorDeps deps)
    : mSystem(system),
      mScale(1.0f),
      mRotation(Quaternion::Identity),
      mState(State::EActive),
      mRecomputeWorldTransform(true) {
    mID = mSystem->AddActor(this);
}

Actor::~Actor() {
    mSystem->RemoveActor(mID);
    while (!mComponents.empty()) delete mComponents.back();
}

void Actor::ProcessInput(const InputState& state) {
    if (mState == State::EActive) {
        for (auto comp : mComponents) {
            comp->ProcessInput(state);
        }
        ActorInput(state);
    }
}

void Actor::Update(float deltatime) {
    if (mState == State::EActive) {
        ComputeWorldTransform();
        UpdateComponent(deltatime);
        UpdateActor(deltatime);
        ComputeWorldTransform();
    }
}

void Actor::UpdateComponent(float deltatime) {
    for (auto component : mComponents) {
        component->Update(deltatime);
    }
}

void Actor::AddComponent(Component* component) {
    int updateOrder = component->GetUpdateOrder();
    auto iter = mComponents.begin();
    for (; iter != mComponents.end(); ++iter) {
        if (updateOrder < (*iter)->GetUpdateOrder()) {
            break;
        }
    }
    mComponents.insert(iter, component);
}

void Actor::RemoveComponent(class Component* component) {
    auto iter = std::find(mComponents.begin(), mComponents.end(), component);
    if (iter != mComponents.end()) {
        mComponents.erase(iter);
    }
}

void Actor::ComputeWorldTransform() {
    if (mRecomputeWorldTransform) {
        mRecomputeWorldTransform = false;
        // scaling, rotation, translation
        mWorldTransform = Matrix4::CreateScale(mScale);
        mWorldTransform *= Matrix4::CreateFromQuaternion(mRotation);
        mWorldTransform *= Matrix4::CreateTranslation(mPosition);
        for (auto comp : mComponents) {
            comp->OnUpdateWorldTransform();
        }
    }
}

/*
Component* Actor::GetComponentOfType(Component::TypeID type) {
    Component* comp = nullptr;

    for (auto c : mComponents) {
        if (c->GetType() == type) {
            comp = c;
            break;
        }
    }
    return comp;
}

void Actor::LoadProperties(const rapidjson::Value& inObj) {
    // Use strings for different states
    std::string state;
    if (JsonHelper::GetString(inObj, "state", state)) {
        if (state == "active") {
            SetState(State::EActive);
        } else if (state == "paused") {
            SetState(State::EPaused);
        } else if (state == "dead") {
            SetState(State::EDead);
        }
    }

    // Load position, rotation, and scale, and compute transform
    JsonHelper::GetVector3(inObj, "position", mPosition);
    JsonHelper::GetQuaternion(inObj, "rotation", mRotation);
    JsonHelper::GetFloat(inObj, "scale", mScale);
    ComputeWorldTransform();
}
*/

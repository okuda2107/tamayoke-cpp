#include "game/object/Component.h"

#include "game/object/Actor.h"
// #include "core/LevelLoader.h"

Component::Component(Actor* owner, int updateOrder)
    : mOwner(owner), mUpdateOrder(updateOrder) {
    mOwner->AddComponent(this);
}

Component::~Component() { mOwner->RemoveComponent(this); }

/*
void Component::LoadProperties(const rapidjson::Value& inObj) {
    JsonHelper::GetInt(inObj, "updateOrder", mUpdateOrder);
}
*/

#include "game/audio/Event.h"

#include "game/audio/EventInstance.h"

Event::Event(class Bank* bank) : mBank(bank) {};
Event::~Event() {
    while (!mInstances.empty()) {
        delete mInstances.back();
    }
}

void Event::AddInstance(class EventInstance* instance) {
    mInstances.push_back(instance);
}

void Event::RemoveInstance(class EventInstance* instance) {
    auto iter = find(mInstances.begin(), mInstances.end(), instance);
    if (iter != mInstances.end()) {
        std::iter_swap(iter, mInstances.end() - 1);
        mInstances.pop_back();
    }
}

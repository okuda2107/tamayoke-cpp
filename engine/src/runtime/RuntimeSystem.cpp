#include "runtime/RuntimeSystem.h"

#include "runtime/RuntimeData.h"

RuntimeSystem::RuntimeSystem() : mTicksCount(0), mIsGameLoop(true) {
    mDeltatime = 0.0f;
}

RuntimeSystem::~RuntimeSystem() { Shutdown(); }

bool RuntimeSystem::Initialize() {
    if (int sdlResult = SDL_InitSubSystem(SDL_INIT_TIMER)) {
        SDL_Log("Failed to Initialize SDL timer:%s", SDL_GetError());
        return false;
    }

    mTicksCount = SDL_GetTicks();

    return true;
}

void RuntimeSystem::Shutdown() { SDL_QuitSubSystem(SDL_INIT_TIMER); }

bool RuntimeSystem::IsRunning() const { return mIsGameLoop; }

void RuntimeSystem::BeginFrame() {
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));
    mDeltatime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    if (mDeltatime > 0.05f) {
        mDeltatime = 0.05f;
    }
    mTicksCount = SDL_GetTicks();
}

void RuntimeSystem::EndFrame() {}

void RuntimeSystem::ProcessGameData(const GameFrameResult& state) {
    mIsGameLoop = state.mIsGameLoop;
    mInputSystemMetrics.mRelativeMouseMode = state.mRelativeMouseMode;
}

#include "Logo.h"

#include "SDL.h"
#include "game/audio/AudioComponent.h"
#include "renderer/RenderDB.h"
#include "renderer/SpriteComponent.h"

Logo::Logo(class ActorsSystem* system, LogoDeps& deps) : Actor(system, deps) {
    // ロゴを出す
    mSpriteComp = new SpriteComponent(this, &deps.renderDB);
    mSpriteComp->SetTexture(deps.renderDB.GetTexture("Assets/title.png"));

    // BGMを流す
    mAudioComp = new AudioComponent(this, deps.audioSystem);
    if (!mAudioComp->RegisterEvent(mEventTag))
        SDL_Log("failed to register title audio");
    auto event = mAudioComp->GetEvent(mEventTag);
    event.Restart();
}

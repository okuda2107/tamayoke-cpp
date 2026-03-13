#include "TamayokeGame.h"

#include "SDL.h"
#include "game/Game.h"
#include "game/RuntimeRequestManager.h"
#include "game/StateManager.h"
#include "game/audio/AudioSystem.h"
#include "game/physics/PhysWorld.h"
#include "input/InputState.h"
#include "renderer/RenderDB.h"
#include "renderer/RenderData.h"

TamayokeGame::TamayokeGame() { mGame = new Game(); }
TamayokeGame::~TamayokeGame() { delete mGame; }

bool TamayokeGame::Initialize() { return mGame->Initialize(); }

void TamayokeGame::ProcessInput(const pose::InputState& state) {
    mFrameResult.mIsGameLoop = !state.isQuitGame;
    mSystems.GetSystem<ActorsSystem>()->ProcessInput(state);
}

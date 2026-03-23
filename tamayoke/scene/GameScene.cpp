#include "GameScene.h"

#include "SceneTag.h"
#include "game/audio/AudioSystem.h"
#include "game/physics/PhysWorld.h"
#include "game/scene/ActorQuery.h"
#include "renderer/RenderDB.h"

void GameScene::LoadActors() {}

void GameScene::UnloadActors() {}

void GameScene::TickRules() {}

std::string GameScene::PollNextScene() { return ""; }

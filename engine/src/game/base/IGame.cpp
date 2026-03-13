#include "game/base/IGame.h"

#include "game/base/InputProcessor.h"
#include "game/base/RenderDataGenerator.h"
#include "game/base/Updater.h"

void IGame::IProcessInput(const struct InputStateBase& state) {
    input->IProcessInput(state);
}

const struct GameDataBase& IGame::IUpdate(
    float deltatime, const struct GameMetricsBase& metrics) {
    return update->IUpdate(deltatime, metrics);
}

const struct RenderDataBase& IGame::IGenerateRenderData() {
    return render->IGenerateRenderData();
}

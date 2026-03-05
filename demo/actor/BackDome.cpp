#include "BackDome.h"

#include "renderer/Mesh.h"
#include "renderer/RenderDB.h"
#include "renderer/Shader.h"
#include "renderer/SkydomeComponent.h"

BackDome::BackDome(ActorsSystem* system, BackDomeDeps deps)
    : Actor(system, ActorDeps{}), mTime(0.0f) {
    SetScale(100.0);
    sc = new SkydomeComponent(this, &deps.renderDB);
    sc->SetMesh(deps.renderDB.GetMesh("Assets/Cube.gpmesh"));
    sc->SetShader(deps.renderDB.GetShader("Shaders/NightSky"));
}

void BackDome::UpdateActor(float deltatime) {
    mTime += deltatime;
    sc->GetShader()->SetActive();
    sc->GetShader()->SetFloatUniform("uTime", mTime);
}

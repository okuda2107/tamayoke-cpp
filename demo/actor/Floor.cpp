#include "Floor.h"

#include "game/object/ActorsSystem.h"
#include "renderer/Mesh.h"
#include "renderer/MeshComponent.h"
#include "renderer/RenderDB.h"
#include "renderer/Shader.h"

Floor::Floor(ActorsSystem* system, FloorDeps deps)
    : Actor(system, ActorDeps{}) {
    SetScale(10.0);
    MeshComponent* mc =
        new MeshComponent(this, &deps.renderDB, RenderConfigID::Opaque);
    mc->SetMesh(deps.renderDB.GetMesh("Assets/Plane.gpmesh"));
    mc->SetShader(deps.renderDB.GetShader("Shaders/Phong"));
    mc->AddTextureIndex(0);
}

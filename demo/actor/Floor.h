#pragma once
#include "game/object/Actor.h"
#include "renderer/RenderDB.h"

struct FloorDeps {
    RenderDB& renderDB;

    FloorDeps(RenderDB& render) : renderDB(render) {};
};

class Floor : public Actor {
   public:
    Floor(class ActorsSystem* system, FloorDeps deps);
};

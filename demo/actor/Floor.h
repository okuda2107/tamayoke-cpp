#pragma once
#include "game/object/Actor.h"

struct FloorDeps {
    class RenderDB& renderDB;

    FloorDeps(class RenderDB& render) : renderDB(render) {};
};

class Floor : public Actor {
   public:
    Floor(class ActorsSystem* system, FloorDeps deps);
};

#pragma once

// Actorの配列を保持し，それらのupdateを保証する
template <typename InputState>
class ObjectsSystemBase {
   protected:
    virtual void UnloadObjects() = 0;

   public:
    ObjectsSystemBase() = default;
    virtual ~ObjectsSystemBase() = default;

    virtual void ProcessInput(const InputState& state) = 0;
    virtual void UpdateObjects(float deltatime) = 0;
};

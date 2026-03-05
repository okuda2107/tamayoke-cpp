#include "game/camera/CameraComponent.h"

#include "core/Math.h"
#include "game/audio/AudioComponent.h"
#include "game/audio/AudioSystem.h"
#include "game/object/Actor.h"
#include "renderer/RenderDB.h"

CameraComponent::CameraComponent(class Actor* owner, CameraCompDeps& ccd,
                                 int updateOrder)
    : Component(owner, updateOrder),
      mRenderDB(ccd.renderDB),
      mAudioComponent(ccd.audioComponent) {}

CameraComponent::~CameraComponent() {}

// viewをrendererとaudio systemに渡す．
void CameraComponent::SetViewMatrix(const class Matrix4& view) {
    mRenderDB.SetViewMatrix(view);
    mAudioComponent.GetSystem()->SetListener(view);
}

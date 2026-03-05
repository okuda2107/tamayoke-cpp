#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "game/object/Component.h"

class SpriteComponent : public Component {
   public:
    SpriteComponent(class Actor* owner, class RenderDB* database,
                    int drawOrder = 100);
    ~SpriteComponent();

    virtual void Draw(class Shader* shader);
    virtual void SetTexture(class Texture* tex);

    const int GetDrawOrder() { return mDrawOrder; }

    TypeID GetType() const { return TypeID::TSpriteComponent; }

   protected:
    class RenderDB* mDatabase;

    class Texture* mTexture;
    int mDrawOrder;
    int mTexHeight;
    int mTexWidth;
};

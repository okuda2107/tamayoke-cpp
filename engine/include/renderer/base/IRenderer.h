#pragma once

class IRenderer {
   public:
    IRenderer() = default;
    virtual ~IRenderer() = default;

    virtual void IDraw(const struct RenderDataBase& data) = 0;
};

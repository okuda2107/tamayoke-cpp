#pragma once
#include <type_traits>

#include "IRenderer.h"

template <typename RenderData>
class RendererBase : public IRenderer {
    static_assert(std::is_base_of<RenderDataBase, RenderData>::value,
                  "RenderData must derive from RenderDataBase");

   public:
    RendererBase() = default;
    virtual ~RendererBase() = default;

    virtual void Draw(const RenderData& data) = 0;

    // Engineで使われるためのエントリポイント
    void IDraw(const struct RenderDataBase& data) final {
        Draw(static_cast<const RenderData&>(data));
    }
};

#pragma once

class RenderDataGeneratorBase {
   public:
    virtual const struct RenderDataBase& IGenerateRenderData() = 0;
};

template <typename T>
class RenderDataGenerator : public RenderDataGeneratorBase {
    virtual const T& GenerateRenderData() = 0;
    const struct RenderDataBase& IGenerateRenderData() final {
        return GenerateRenderData();
    }
};

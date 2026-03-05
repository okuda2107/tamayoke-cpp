#include "RuntimeData.h"
#include "SDL.h"
#include "base/RuntimeSystemBase.h"

using Metrics = MetricsBundle<GameMetricsBase, InputSystemMetrics>;

class RuntimeSystem : public RuntimeSystemBase<GameFrameResult, Metrics> {
    Uint32 mTicksCount;
    bool mIsGameLoop;

    void Shutdown();

   public:
    RuntimeSystem();
    ~RuntimeSystem();

    bool Initialize();

    bool IsRunning() const override;

    void BeginFrame() override;
    void EndFrame() override;

    void ProcessGameData(const GameFrameResult& state);
};

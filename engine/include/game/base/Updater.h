#pragma once

class UpdaterBase {
   public:
    virtual const struct GameDataBase& IUpdate(
        float deltatime, const struct GameMetricsBase& metrics) = 0;
};

template <typename TData, typename TMetrics>
class Updater : public UpdaterBase {
   public:
    virtual const TData& Update(float deltatime, const TMetrics& metrics) = 0;
    const struct GameDataBase& IUpdate(float deltatime,
                                       const struct GameMetricsBase& metrics) {
        return Update(deltatime, static_cast<const TMetrics&>(metrics));
    }
};

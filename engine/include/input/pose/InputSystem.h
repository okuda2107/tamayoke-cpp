#pragma once
#include "../base/InputSystemBase.h"
#include "onnxruntime_cxx_api.h"
#include "opencv2/opencv.hpp"

namespace pose {
class InputSystem
    : public InputSystemBase<pose::InputSystem, InputSystemMetricsBase> {
    cv::VideoCapture mCamera;
    Ort::Env mEnv;
    Ort::Session* mSession;
    Ort::SessionOptions mSessionOptions;

    void Shutdown();

   public:
    InputSystem();
    ~InputSystem();

    bool Initialize();

    void Update();

    void ProcessMetrics(const InputSystemMetricsBase& metrics) {}
};
}  // namespace pose

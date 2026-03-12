#pragma once
#include "../base/InputSystemBase.h"
#include "onnxruntime_cxx_api.h"
#include "opencv2/opencv.hpp"

namespace pose {
class InputSystem
    : public InputSystemBase<pose::InputSystem, InputSystemMetricsBase> {
    static constexpr std::wstring_view modelFIlepath =
        L"Assets/AImodel/yolo26n-pose.onnx";
    static const int inputSize = 640;

    static constexpr char* inputNames[] = {"image"};
    static constexpr char* outputNames[] = {"output0"};

    cv::VideoCapture mCamera;
    Ort::Env mEnv;
    Ort::Session* mSession;
    Ort::SessionOptions mSessionOptions;

    std::array<float, inputSize * inputSize * 3> mInputTensorBuf;
    Ort::Value mInputTensor;

    // 入力を処理するヘルパー関数群
    // YOLOの入力フォーマットに合わせるため，640x640のサイズにletterbox処理を行う
    void formatImage(cv::Mat& input, cv::Mat& out);

    // 640x640にフォーマットした画像を入力Tensorバッファに詰める
    void copyImgData(cv::Mat& formattedInput,
                     std::array<float, inputSize * inputSize * 3>& out);

   public:
    InputSystem();
    ~InputSystem();

    bool Initialize();

    void Update();

    void ProcessMetrics(const InputSystemMetricsBase& metrics) {}
};
}  // namespace pose

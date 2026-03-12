#pragma once
#include "../base/InputSystemBase.h"
#include "InputState.h"
#include "onnxruntime_cxx_api.h"
#include "opencv2/opencv.hpp"
#include "runtime/base/MetricsBase.h"

namespace pose {
class InputSystem
    : public InputSystemBase<pose::InputState, InputSystemMetricsBase> {
    // modelデータのpath
    static constexpr std::wstring_view modelFIlepath =
        L"Assets/AImodel/yolo26n-pose.onnx";
    // modelが期待する正方形画像サイズの1辺の長さ
    static const int inputSize = 640;

    // onnx上でのmodelの入出力名
    static constexpr char* inputNames[] = {"image"};
    static constexpr char* outputNames[] = {"output"};

    // 推定結果のしきい値
    static constexpr float detectionConfThreshold = 0.25f;

    // マッピングするScreen Size
    float mScreenWidth;
    float mScreenHeight;

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
    InputSystem(float screenWidth, float screenHeight);
    ~InputSystem();

    bool Initialize();

    void Update();

    void ProcessMetrics(const InputSystemMetricsBase& metrics) {}
};
}  // namespace pose

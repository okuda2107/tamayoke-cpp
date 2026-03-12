#include "input/pose/InputSystem.h"

namespace pose {
InputSystem::InputSystem() {
    // 実行環境設定の指定，modelのロード
    mEnv = Ort::Env(ORT_LOGGING_LEVEL_WARNING, "pose");
    mSessionOptions.SetIntraOpNumThreads(1);
    mSession = new Ort::Session(mEnv, modelFIlepath.data(), mSessionOptions);

    // 入力Tensor作成

    // format指定構造体とメモリ属性指定構造体は内部でコピーされるので，ローカル変数で良い

    // Tensorデータのformat指定
    // YOLOの入力は {batch, channel, height, width}
    // batchとは入力画像の枚数を表す
    std::array<int64_t, 4> inputTensorFormat{1, 3, inputSize, inputSize};

    // メモリ属性の識別子を指定
    Ort::MemoryInfo inputTensorMemoryInfo =
        Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeDefault);

    Ort::Value inputTensor = Ort::Value::CreateTensor<float>(
        inputTensorMemoryInfo, mInputTensorBuf.data(), mInputTensorBuf.size(),
        inputTensorFormat.data(), inputTensorFormat.size());
}

InputSystem::~InputSystem() {
    delete mSession;
    // カメラの終了処理
}

bool InputSystem::Initialize() {
    // カメラの初期化
    return true;
}

void InputSystem::Update() {
    // カメラから画像を取得
    cv::Mat input;

    cv::Mat resizedImg;
    formatImage(input, resizedImg);

    // 値を正規化
    resizedImg.convertTo(resizedImg, CV_32F, 1.0 / 255.0);

    copyImgData(resizedImg, mInputTensorBuf);

    // 出力
    auto outputTensors = mSession->Run(Ort::RunOptions{nullptr}, inputNames,
                                       &mInputTensor, 1, outputNames, 1);

    /*
    modelからの出力メモ:
    data size: [1, 300, 57]: [batch, detections, attributes]
    batch: 画像の枚数
    detections: 検出結果の数．検出結果の候補が並んでる
    attributes: 1つの検出結果が持つデータ数
        bounding box 0-3 x1, y1, x2, y2のboxの左上と右上の座標
        confidence 4: それがどれくらい確からしいか
        class 5: 物体の種類: person class = 0となることが多い
        keypoints 6-57: 17個のkeypoint: 1つのkeypointにx, y, confidence
            順番:
            0 nose
            1 left_eye
            2 right_eye
            3 left_ear
            4 right_ear
            5 left_shoulder
            6 right_shoulder
            7 left_elbow
            8 right_elbow
            9 left_wrist
            10 right_wrist
            11 left_hip
            12 right_hip
            13 left_knee
            14 right_knee
            15 left_ankle
            16 right_ankle
    */
    float* output = outputTensors[0].GetTensorMutableData<float>();
}

void InputSystem::formatImage(cv::Mat& input, cv::Mat& out) {
    int imgW = input.cols;
    int imgH = input.rows;

    // 入力画像の幅と高さを見て，大きい方の辺を640にするscaleを採用する
    int maxEdge = std::max(imgW, imgH);
    float scale = (float)inputSize / maxEdge;

    // 長辺が640となるようにリサイズする
    int formattedW = int(imgW * scale);
    int formattedH = int(imgH * scale);

    cv::Mat resized;
    cv::resize(input, resized, cv::Size(formattedW, formattedH));

    // 画像が640x640となるようにpaddingする
    int padW = inputSize - formattedW;
    int padH = inputSize - formattedH;

    int top = padH / 2;
    int bottom = padH - top;
    int left = padW / 2;
    int right = padW - left;

    cv::copyMakeBorder(resized, out, top, bottom, left, right,
                       cv::BORDER_CONSTANT, cv::Scalar(114, 114, 114));
}

void InputSystem::copyImgData(
    cv::Mat& formattedInput,
    std::array<float, inputSize * inputSize * 3>& out) {
    /*
        note:
        cv::MatとYOLOが期待する入力Tensorのフォーマットが異なることに注意
        行列の並びに関して，
        cv::Matは {height, width, channel} であるのに対し，
        YOLOの入力は {batch, channel, height, width}
        batchとは入力画像の枚数を表す

        多次元行列を1次元配列として解釈する場合，
        右の添字を先に0→maxまで動かし，次に左の添字を増やす順 (row-major) でメモリに並ぶ

        具体的な配列の並びメモ
        cv::Mat
            {{座標11のBGRの3値}, {座標12のBGRの3値}, ...}
        YOLOの入力
            {{画像のR成分のh*w個の画素値}, {画像のG成分のh*w個の画素値}, ...}
    */
    // strideを画像のサイズとする
    int imageSize = inputSize * inputSize;
    const float* ptr = formattedInput.ptr<float>(0);

    for (int i = 0; i < imageSize; i++) {
        out[i] = ptr[i * 3 + 2];
        out[imageSize + i] = ptr[i * 3 + 1];
        out[2 * imageSize + i] = ptr[i * 3];
    }
}
}  // namespace pose

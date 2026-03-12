#include <iostream>

#include "Engine.h"
#include "SDL.h"
#include "onnxruntime_cxx_api.h"
#include "opencv2/opencv.hpp"

// ゲーム実装者がシステムの内容を定義
int main(int argc, char** argv) {
    // --- onnx demo ---
    // グローバル環境設定
    Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "pose");

    // セッション設定，推論の設定を実際にどうするか
    Ort::SessionOptions sessionOptions;
    sessionOptions.SetIntraOpNumThreads(1);

    // モデルのロード
    Ort::Session session(env, L"Assets/AIModel/yolo26n-pose.onnx",
                         sessionOptions);

    int inputSize = 640;

    // Tensorバッファ
    std::vector<float> inputTensorValues(inputSize * inputSize * 3);

    // 入力Tensor形状の情報
    // {batch, channels, height, width}
    std::array<int64_t, 4> inputShape{1, 3, inputSize, inputSize};

    // MemoryInfoはそのメモリの使い方を指定する．どのデバイスのメモリにあるか，どのアロケータで管理されているかを指定するモノ．メモリ属性の識別子
    // allocatorはメモリを確保，解放などの管理を行う仕組み
    Ort::MemoryInfo memoryInfo =
        Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeDefault);
    // Tensor作成
    Ort::Value inputTensor = Ort::Value::CreateTensor<float>(
        memoryInfo, inputTensorValues.data(), inputTensorValues.size(),
        inputShape.data(), inputShape.size());

    // 入出力名 ONNX graphのnode名
    const char* inputNames[] = {"images"};
    const char* outputNames[] = {"output0"};
    // 画像の読み込み
    // cv::MatはHWC layout
    // Height, Width, Channel
    // [H][W][C]

    // resize
    // YOLOは固定サイズ入力なので640x640に変換．本来はletterboxを使う
    // letterboxとは: アスペクト比を変えずにリサイズして，paddingは埋める方式
    cv::VideoCapture cap(1);
    if (!cap.isOpened()) {
        std::cerr << "failed" << std::endl;
        return -1;
    }
    cv::Mat img;
    while (true) {
        cap >> img;
        int imgW = img.cols;
        int imgH = img.rows;

        float scale =
            std::min((float)inputSize / imgW, (float)inputSize / imgH);

        int newW = int(imgW * scale);
        int newH = int(imgH * scale);

        cv::Mat resized;
        cv::resize(img, resized, cv::Size(newW, newH));

        int padW = inputSize - newW;
        int padH = inputSize - newH;

        int top = padH / 2;
        int bottom = padH - top;
        int left = padW / 2;
        int right = padW - left;

        cv::Mat padded;
        cv::copyMakeBorder(resized, padded, top, bottom, left, right,
                           cv::BORDER_CONSTANT, cv::Scalar(114, 114, 114));

        // 正規化．0-255 から 0-1にしている
        padded.convertTo(padded, CV_32F, 1.0 / 255.0);

        // データコピー
        // ここでcv::MatのレイアウトとYOLOモデルの期待しているレイアウトが違うことに注意
        // YOLOモデルはNCHW，[batch][channel][height][width]
        int channelSize = inputSize * inputSize;

        const float* ptr = padded.ptr<float>(0);

        for (int i = 0; i < inputSize * inputSize; i++) {
            inputTensorValues[i] = ptr[i * 3 + 2];
            inputTensorValues[channelSize + i] = ptr[i * 3 + 1];
            inputTensorValues[2 * channelSize + i] = ptr[i * 3 + 0];
        }

        auto outputTensors = session.Run(Ort::RunOptions{nullptr}, inputNames,
                                         &inputTensor, 1, outputNames, 1);

        // ouitputTensorsはvectorモデルから複数の値が出てくる可能性があるので[0]番指定
        // 今回はRunの時に引数で指定してる
        float* output = outputTensors[0].GetTensorMutableData<float>();

        /*
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

        int stride = 57;

        for (int i = 0; i < 300; i++) {
            float conf = output[i * stride + 4];
            if (conf < 0.25) continue;

            //--------------------------------
            // bbox
            //--------------------------------

            float x1 = output[i * stride + 0];
            float y1 = output[i * stride + 1];
            float x2 = output[i * stride + 2];
            float y2 = output[i * stride + 3];

            // letterbox逆変換
            // padding補正
            x1 -= left;
            x2 -= left;
            y1 -= top;
            y2 -= top;

            // scale戻す
            x1 /= scale;
            x2 /= scale;
            y1 /= scale;
            y2 /= scale;

            cv::rectangle(img, cv::Point(x1, y1), cv::Point(x2, y2),
                          cv::Scalar(0, 255, 0), 2);

            //--------------------------------
            // keypoints
            //--------------------------------

            std::vector<cv::Point> kpts(17);

            for (int k = 0; k < 17; k++) {
                float kx = output[i * stride + 6 + k * 3 + 0];
                float ky = output[i * stride + 6 + k * 3 + 1];
                float kc = output[i * stride + 6 + k * 3 + 2];

                if (kc < 0.3) continue;

                kx -= left;
                ky -= top;

                kx /= scale;
                ky /= scale;

                kpts[k] = cv::Point(kx, ky);

                cv::circle(img, kpts[k], 4, cv::Scalar(0, 0, 255), -1);
            }

            //--------------------------------
            // skeleton
            //--------------------------------

            const int skeleton[][2] = {{0, 1},   {0, 2},   {1, 3},   {2, 4},
                                       {5, 6},   {5, 7},   {7, 9},   {6, 8},
                                       {8, 10},  {5, 11},  {6, 12},  {11, 12},
                                       {11, 13}, {13, 15}, {12, 14}, {14, 16}};
            for (auto& s : skeleton) {
                int a = s[0];
                int b = s[1];

                if (kpts[a] == cv::Point() || kpts[b] == cv::Point()) continue;

                cv::line(img, kpts[a], kpts[b], cv::Scalar(255, 0, 0), 2);
            }
        }
        cv::imshow("pose", img);

        int key = cv::waitKey(1);
        if (key == 27) {
            break;
        }
    }
    cap.release();
    cv::destroyAllWindows();
    // --- onnx demo ここまで ---

    return 0;
}

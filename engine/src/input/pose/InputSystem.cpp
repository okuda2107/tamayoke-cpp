#include "input/pose/InputSystem.h"

namespace pose {
InputSystem::InputSystem() {
    mEnv = Ort::Env(ORT_LOGGING_LEVEL_WARNING, "pose");
    mSessionOptions.SetIntraOpNumThreads(1);
    mSession = new Ort::Session(mEnv, modelFIlepath.data(), mSessionOptions);
}

InputSystem::~InputSystem() {
    delete mSession;
    // カメラの終了処理
}

bool InputSystem::Initialize() {
    // カメラの初期化
    return true;
}

void Update() {
    // カメラから画像を取得
}
}  // namespace pose

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

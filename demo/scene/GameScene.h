#pragma once
#include "BonfireSceneBase.h"
#include "game/object/Actor.h"

class GameScene : public BonfireSceneBase {
    enum State {
        EEnterAnim,
        EPlay,
        EOutAnim,
    };

    State mState;

    ActorID mTimerID;

    // 前回生成Woodを生成したときの時間
    float mPrevTime;

    ActorID mPlayerID;
    ActorID mBonfireID;
    std::vector<ActorID> mWoodIDs;

    // 次のSceneに移る
    bool mIsNextScene;

   public:
    GameScene()
        : BonfireSceneBase(),
          mState(State::EEnterAnim),
          mIsNextScene(false),
          mPlayerID(-1),
          mBonfireID(-1),
          mPrevTime(0.0f) {};

    void LoadActors();
    void UnloadActors();

    void TickRules();

    std::string PollNextScene();
};

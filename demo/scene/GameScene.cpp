#include "GameScene.h"

#include "../actor/Bonfire.h"
#include "../actor/BonfirePlayer.h"
#include "../actor/Timer.h"
#include "../actor/Wood.h"
#include "BonfireSceneTag.h"
#include "core/Random.h"
#include "game/scene/ActorQuery.h"
#include "game/scene/SceneManager.h"

void GameScene::LoadActors() {
    // Timerのロード
    mTimerID = mActorQuery->CreateActor<Timer, TimerDeps, TypeLists<>>();

    // Player と Bonfire を取得
    auto playerIDContext = mSceneManagerData->at(mPlayerIDTag);
    mPlayerID = playerIDContext.actorID;
    auto bonfireIDContext = mSceneManagerData->at(mBonfireIDTag);
    mBonfireID = bonfireIDContext.actorID;

    auto player = mActorQuery->GetActor<BonfirePlayer>(mPlayerID);
    auto bonfire = mActorQuery->GetActor<Bonfire>(mBonfireID);
    if (!player || !bonfire)
        throw std::runtime_error("Invalid Playaer or Bonfire Object");

    // ゲームスタートを各Actorに通知
    player->SetAnimLookDown();
    bonfire->SetRunning();
}

void GameScene::UnloadActors() {
    auto timer = mActorQuery->GetActor<Timer>(mTimerID);
    if (timer) timer->SetState(Actor::State::EDead);
    mState = State::EEnterAnim;
    mIsNextScene = false;
    mPlayerID = -1;
    mBonfireID = -1;
    for (auto id : mWoodIDs) {
        Wood* wd = mActorQuery->GetActor<Wood>(id);
        if (wd) wd->SetState(Actor::State::EDead);
    }
    mWoodIDs.clear();
    mPrevTime = 0.0f;
}

void GameScene::TickRules() {
    if (mState == State::EEnterAnim) {
        auto player = mActorQuery->GetActor<BonfirePlayer>(mPlayerID);
        if (!player) throw std::runtime_error("Invalid Playaer Object");

        if (!player->GetAnimLookDown()) {
            mState = State::EPlay;
        }

    } else if (mState == State::EPlay) {
        auto timer = mActorQuery->GetActor<Timer>(mTimerID);
        if (!timer) throw std::runtime_error("Invalid Timer Object");

        // 1秒ごとに生成判定
        if (timer->mTime - mPrevTime > 1.0f) {
            mPrevTime = timer->mTime;
            // 逆数関数的に確率が減少
            float T = 30.0f;
            float n = 2.0f;
            float prob = 1.0f / powf(1.0f + (timer->mTime / T), n);
            if (Random::GetFloat() < prob) {
                ActorID id =
                    mActorQuery->CreateActor<Wood, WoodDeps,
                                             TypeLists<RenderDB, PhysWorld>>();
                mWoodIDs.push_back(id);
                Wood* wd = mActorQuery->GetActor<Wood>(id);
                wd->SetPosition(Vector3(
                    Random::GetFloatRange(mFieldMin.x, mFieldMax.x),
                    Random::GetFloatRange(mFieldMin.y, mFieldMax.y), -50));
            }

            // 既に死んでいるWoodは配列から除外
            for (auto iter = mWoodIDs.begin(); iter != mWoodIDs.end();) {
                Wood* wd = mActorQuery->GetActor<Wood>(*iter);
                if (!wd)
                    mWoodIDs.erase(iter);
                else
                    ++iter;
            }
        }

        auto bonfire = mActorQuery->GetActor<Bonfire>(mBonfireID);
        auto player = mActorQuery->GetActor<BonfirePlayer>(mPlayerID);
        if (!player || !bonfire)
            throw std::runtime_error("Invalid Playaer or Bonfire Object");

        if (bonfire->GetFinished()) {
            bonfire->SetState(Actor::State::EDead);
            player->SetAnimLookUp();
            mState = State::EOutAnim;
        }

    } else if (mState == State::EOutAnim) {
        auto player = mActorQuery->GetActor<BonfirePlayer>(mPlayerID);
        if (!player) throw std::runtime_error("Invalid Playaer Object");

        if (!player->GetAnimLookUp()) {
            mIsNextScene = true;
            auto timer = mActorQuery->GetActor<Timer>(mTimerID);
            int time = -2;
            if (timer) time = timer->mTime;
            SceneContext data{};
            data.number = time;
            mSceneManagerData->emplace(mResultTag, data);
        }
    }
}

std::string GameScene::PollNextScene() {
    if (mIsNextScene)
        return SceneName::result.data();
    else
        return "";
}

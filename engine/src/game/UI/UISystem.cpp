#include "game/UI/UISystem.h"

#include "game/UI/UIScreen.h"

UIID UISystem::PushUI(UIScreen* ui) {
    mUIStack.emplace_back(ui);

    // mapへの登録
    while (true) {
        auto iter = mLendingMap.find(mID);
        if (iter != mLendingMap.end())
            mID++;
        else
            break;
    }
    mLendingMap.emplace(mID, ui);
    return mID;
}

void UISystem::Update(float deltatime) {
    for (auto ui : mUIStack) {
        if (ui->GetState() == UIScreen::EActive) ui->Update(deltatime);
    }
    auto iter = mUIStack.begin();
    while (iter != mUIStack.end()) {
        if ((*iter)->GetState() == UIScreen::EClosing) {
            // mapから削除
            UIID id = (*iter)->GetID();
            delete *iter;
            iter = mUIStack.erase(iter);
            mLendingMap.erase(id);
        } else {
            ++iter;
        }
    }
}

#pragma once
#include <cstdint>
#include <unordered_map>
#include <vector>

typedef uint32_t UIID;

class UISystem {
    std::vector<class UIScreen*> mUIStack;

    std::unordered_map<UIID, class UIScreen*> mLendingMap;

    UIID mID = 0;

   public:
    std::vector<class UIScreen*>& GetUIStack() { return mUIStack; }
    UIID PushUI(class UIScreen* screen);

    void Update(float deltatime);

    template <typename T>
    T* GetUI(UIID id) {
        auto iter = mLendingMap.find(id);
        if (iter == mLendingMap.end()) return nullptr;

        // cast
        return dynamic_cast<T*>(iter->second);
    }
};

#pragma once

#include <list>
#include <memory>

class GameObject;
class Time;

class FriedChickenManager {
    using GameObjectPtr = std::shared_ptr<GameObject>;
    using GameObjectPtrList = std::list<GameObjectPtr>;

public:
    FriedChickenManager();
    ~FriedChickenManager();
    void initialize();
    //ターゲットから一番近い唐揚げを検索
    GameObjectPtr FindNearestChicken(const GameObjectPtr target);
    //excludeを除くターゲットから一番近い唐揚げを検索
    GameObjectPtr FindNearestChicken(const GameObjectPtr target, const GameObjectPtr exclude);

private:
    //揚げてる最中の唐揚げ
    GameObjectPtrList mChickens;
    //待機中の唐揚げ
    GameObjectPtrList mWaitingChickens;
    std::unique_ptr<Time> mSpawnTimer;
    //一度に画面内に表示する最大数
    int mMaxDrawNum;
};

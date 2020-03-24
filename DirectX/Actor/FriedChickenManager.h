#pragma once

#include <list>
#include <memory>

class Actor;
class FriedChicken;
class Time;

class FriedChickenManager {
    using FriedChickenPtr = std::shared_ptr<FriedChicken>;
    using FriedChickenPtrList = std::list<FriedChickenPtr>;

public:
    FriedChickenManager();
    ~FriedChickenManager();
    void initialize();
    //ターゲットから一番近い唐揚げを検索
    FriedChickenPtr FindNearestChicken(const std::shared_ptr<Actor> target);
    //excludeを除くターゲットから一番近い唐揚げを検索
    FriedChickenPtr FindNearestChicken(const std::shared_ptr<Actor> target, const FriedChickenPtr exclude);

private:
    //揚げてる最中の唐揚げ
    FriedChickenPtrList mChickens;
    //待機中の唐揚げ
    FriedChickenPtrList mWaitingChickens;
    std::unique_ptr<Time> mSpawnTimer;
    //一度に画面内に表示する最大数
    int mMaxDrawNum;
};

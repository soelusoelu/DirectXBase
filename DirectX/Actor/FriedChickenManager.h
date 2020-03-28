#pragma once

#include <list>
#include <memory>

class Actor;
class Time;

class FriedChickenManager {
    using ActorPtr = std::shared_ptr<Actor>;
    using ActorPtrList = std::list<ActorPtr>;

public:
    FriedChickenManager();
    ~FriedChickenManager();
    void initialize();
    //ターゲットから一番近い唐揚げを検索
    ActorPtr FindNearestChicken(const ActorPtr target);
    //excludeを除くターゲットから一番近い唐揚げを検索
    ActorPtr FindNearestChicken(const ActorPtr target, const ActorPtr exclude);

private:
    //揚げてる最中の唐揚げ
    ActorPtrList mChickens;
    //待機中の唐揚げ
    ActorPtrList mWaitingChickens;
    std::unique_ptr<Time> mSpawnTimer;
    //一度に画面内に表示する最大数
    int mMaxDrawNum;
};

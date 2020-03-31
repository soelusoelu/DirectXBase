#pragma once

#include <list>
#include <memory>

class FriedChickenComponent;
class GameObject;
class Score;
class Time;

class FriedChickenManager {
    using ChickenPtr = std::shared_ptr<FriedChickenComponent>;
    using ChickenPtrList = std::list<ChickenPtr>;
    using GameObjectPtr = std::shared_ptr<GameObject>;

public:
    FriedChickenManager();
    ~FriedChickenManager();
    void initialize();
    void update();
    //ターゲットから一番近い唐揚げを検索
    GameObjectPtr FindNearestChicken(const GameObjectPtr target);
    //excludeを除くターゲットから一番近い唐揚げを検索
    GameObjectPtr FindNearestChicken(const GameObjectPtr target, const GameObjectPtr exclude);
    //スコアコンポーネントをセット
    void setScore(const GameObjectPtr score);

private:
    void moveToWait();

private:
    //揚げてる最中の唐揚げ
    ChickenPtrList mChickens;
    //待機中の唐揚げ
    ChickenPtrList mWaitingChickens;
    std::unique_ptr<Time> mSpawnTimer;
    //一度に画面内に表示する最大数
    int mMaxDrawNum;
    //スコア
    std::shared_ptr<Score> mScore;
};

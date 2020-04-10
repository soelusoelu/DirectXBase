#pragma once

#include "Component.h"
#include <list>
#include <memory>

class FriedChickenComponent;
class GameObject;
class ScoreEvaluation;
class Time;

class FriedChickenManager : public Component {
    using ChickenPtr = std::shared_ptr<FriedChickenComponent>;
    using ChickenPtrList = std::list<ChickenPtr>;
    using GameObjectPtr = std::shared_ptr<GameObject>;

public:
    FriedChickenManager(std::shared_ptr<GameObject> owner);
    ~FriedChickenManager();
    virtual void awake() override;
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(debugInfoList* inspect) const override;
    //ターゲットから一番近い唐揚げを検索
    GameObjectPtr FindNearestChicken(const GameObjectPtr target);
    //excludeを除くターゲットから一番近い唐揚げを検索
    GameObjectPtr FindNearestChicken(const GameObjectPtr target, const GameObjectPtr exclude);

private:
    //揚げ終わった唐揚げの状態に応じてスコアを加算
    void addScore();
    //揚げ終わった唐揚げを回収する
    void moveToWait();
    //唐揚げを補充する
    void replenish();

private:
    std::shared_ptr<ScoreEvaluation> mScoreEvaluation;
    //揚げてる最中の唐揚げ
    ChickenPtrList mChickens;
    //待機中の唐揚げ
    ChickenPtrList mWaitingChickens;
    std::unique_ptr<Time> mSpawnTimer;
    //一度に画面内に表示する最大数
    int mMaxDrawNum;
};

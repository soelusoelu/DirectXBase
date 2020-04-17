#pragma once

#include "Component.h"
#include <list>
#include <memory>

class FriedChickenComponent;
class GameObject;
class Score;
class ScoreEvaluation;

class FriedChickenManager : public Component {
    using ChickenPtr = std::shared_ptr<FriedChickenComponent>;
    using ChickenPtrList = std::list<ChickenPtr>;
    using GameObjectPtr = std::shared_ptr<GameObject>;
    using GameObjectPtrList = std::list<GameObjectPtr>;

public:
    FriedChickenManager(std::shared_ptr<GameObject> owner);
    ~FriedChickenManager();
    virtual void awake() override;
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(DebugInfoList* inspect) const override;
    //ターゲットから一番近い唐揚げを検索
    ChickenPtr findNearestChicken(const GameObjectPtr& target) const;
    //excludeを除くターゲットから一番近い唐揚げを検索
    ChickenPtr findNearestChicken(const GameObjectPtr& target, const ChickenPtr& exclude) const;
    //揚げてる最中の唐揚げをすべて取得
    GameObjectPtrList getFriedChickens() const;

private:
    //揚げ終わった唐揚げの状態に応じてスコアを加算
    void addScore();
    //揚げ終わった唐揚げを回収する
    void moveToWait();
    //唐揚げを補充する
    void replenish();

private:
    std::shared_ptr<Score> mScore;
    std::shared_ptr<ScoreEvaluation> mScoreEvaluation;
    //揚げてる最中の唐揚げ
    ChickenPtrList mChickens;
    //待機中の唐揚げ
    ChickenPtrList mWaitingChickens;
    //一度に画面内に表示する最大数
    int mMaxDrawNum;
};

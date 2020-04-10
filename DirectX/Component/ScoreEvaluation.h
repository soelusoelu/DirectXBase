#pragma once

#include "Component.h"
#include <memory>

class FriedChickenComponent;
class Score;

class ScoreEvaluation : public Component {
public:
    ScoreEvaluation(std::shared_ptr<GameObject> owner);
    ~ScoreEvaluation();
    virtual void start() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(debugInfoList* inspect) const override;
    //揚げ状態によってスコアを評価
    void evaluateScore(const FriedChickenComponent& chicken);
    //good時間比率の取得
    float getGood() const;

private:
    std::shared_ptr<Score> mScore;
    float mLittleBad;
    float mUsually;
    float mGood;
    int mLittleBadScore;
    int mUsuallyScore;
    int mGoodScore;
    int mBadScore;
};

#pragma once

#include "Component.h"
#include "IChickenFry.h"

//唐揚げの揚げ具合からスコアを評価するクラス
class ScoreEvaluation : public Component {
public:
    ScoreEvaluation(std::shared_ptr<GameObject> owner);
    ~ScoreEvaluation();
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(DebugInfoList* inspect) const override;
    //揚げ状態によってスコアを評価
    int evaluateScore(const IChickenFry& chicken) const;

private:
    int mLittleBadScore;
    int mUsuallyScore;
    int mGoodScore;
    int mBadScore;
};

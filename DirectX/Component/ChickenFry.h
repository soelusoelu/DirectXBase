﻿#pragma once

#include "Component.h"
#include "ChickenSurface.h"
#include "IChickenFry.h"
#include <memory>
#include <string>
#include <vector>

class ChickenColorChanger;
class Time;

class ChickenFry : public Component, public IChickenFry {
    using TimerPtr = std::shared_ptr<Time>;
    using TimerPtrArray = std::vector<TimerPtr>;

public:
    ChickenFry(std::shared_ptr<GameObject> owner);
    ~ChickenFry();
    virtual void awake() override;
    virtual void start() override;
    virtual void onUpdateWorldTransform() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(DebugInfoList* inspect) const override;

    virtual void initialize() override;
    virtual void update() override;
    virtual bool isBurntAllSurfaces() const override;
    virtual bool isTooBurnt() const override;
    virtual bool isBurntHalfSurfaces() const override;
    virtual FryState getFryState(ChickenSurface surface) const override;
    virtual FryState getFryState(unsigned surfaceIndex) const override;

private:
    //揚がりやすい・揚がりにくい面を決める
    void choiceEasyAndHardSurface();
    //下の面を決定する
    void choiceBottomSurface();
    //下の面を揚げる
    void frying();
    //下の面が焦げてたらタイマーを進める
    void updateTimerIfBurntBottomSurface();
    //面の数の取得
    int getNumSurface() const;
    //揚げ状態の数の取得
    int getNumFryState() const;
    //Surfaceを文字列化
    std::string surfaceToString(ChickenSurface surface) const;

private:
    std::shared_ptr<ChickenColorChanger> mColorChanger;
    TimerPtrArray mFryTimer;
    ChickenSurface mCurrentBottomSurface;
    ChickenSurface mEasySurface;
    ChickenSurface mHardSurface;
    std::unique_ptr<Time> mTooBurntTimer;

    //普通の面
    std::vector<float> mUsually;
    //揚がりやすい面
    std::vector<float> mEasy;
    //揚がりにくい面
    std::vector<float> mHard;

    //焦げるまでの時間
    float mBurntTime;
    float mEasyBurntTime;
    float mHardBurntTime;
};

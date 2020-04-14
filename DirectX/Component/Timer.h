#pragma once

#include "Component.h"

class Timer : public Component {
public:
    Timer(std::shared_ptr<GameObject> owner);
    ~Timer();
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const override;
    virtual void drawDebugInfo(DebugInfoList* inspect) const override;
    //設定を最初の状態に戻す
    void reset();
    //カウントアップタイムがリミットタイムを超えたか
    bool isTime() const;
    //リミットタイムの再設定
    void setLimitTime(float sec);
    //現在のカウントアップタイムを返す
    float currentUpTime() const;
    //現在のカウントダウンタイムを返す
    float currentDownTime() const;
    //リミットタイムを返す
    float limitTime() const;
    //リミットタイムに対してのカウントアップタイムの比率
    float rate() const;

private:
    float mCurrentTime;
    float mLimitTime;
    bool mIsOverLimit;
};

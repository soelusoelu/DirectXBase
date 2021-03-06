﻿#pragma once

#include "Component.h"
#include "IChickenFry.h"
#include "../Math/Math.h"
#include <memory>

class ChickenRise;

class FriedChickenComponent : public Component {
    enum class State {
        FRY,
        FALL,
        RISE,
        TOO_BURNT,
        EATEN
    };

public:
    FriedChickenComponent();
    ~FriedChickenComponent();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;
    const IChickenFry& getFry() const;
    //揚げ直す前の状態に戻す
    void initialize();
    //強制的に揚げ終わる
    void finishFryed();
    //スコア加算待ち状態に移行
    void setWaitingScore(bool value);
    //上昇モード突入
    void setUp();
    //食われる
    void eaten();
    //その場で唐揚げを回転させる
    void roll(const Vector3& direction);

    //揚げている途中か
    bool isFrying() const;
    //落ちている途中か
    bool isFalling() const;
    //上昇している途中か
    bool isRise() const;
    //揚げ終わっているか
    bool isFinished() const;
    //スコア加算待ちか
    bool isWaitingScore() const;
    //食われたか
    bool isEaten() const;
    //焦げすぎたか
    bool isTooBurnt() const;

private:
    //焦げすぎた場合
    void tooBurnt();
    void tooBurntUpdate();
    //揚げ終わったら状態遷移させる
    void autoCollection();
    //空中から落下させる
    void fall();
    //油に浸かったら揚げ始める
    void soakedInOil();

private:
    std::shared_ptr<IChickenFry> mFry;
    std::shared_ptr<ChickenRise> mRise;
    State mState;
    Vector2 mRandomRangePositionX;
    Vector2 mRandomRangePositionZ;
    Vector2 mRandomRangeScale;
    float mRollSpeed;
    float mFallSpeed;
    bool mIsWaitingColliction;
    bool mIsWaitingAddScore;
};

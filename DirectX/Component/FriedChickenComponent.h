#pragma once

#include "Component.h"
#include "IChickenFry.h"
#include "../Math/Math.h"
#include <memory>

class FriedChickenComponent : public Component {
    enum class State {
        FRY,
        FALL,
        UP,
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
    bool isUP() const;
    //揚げ終わっているか
    bool isFinished() const;
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
    //上昇させる
    void up();
    //上昇終了
    void upEnd();
    //油に浸かったら揚げ始める
    void soakedInOil();

private:
    std::shared_ptr<IChickenFry> mFry;
    State mState;
    Vector2 mRandomRangePositionX;
    Vector2 mRandomRangePositionZ;
    Vector2 mRandomRangeScale;
    float mRollSpeed;
    float mFallSpeed;
    float mUpSpeed;
    bool mIsWaitingColliction;
};

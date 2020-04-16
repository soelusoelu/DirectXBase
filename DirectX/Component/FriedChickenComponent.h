#pragma once

#include "Component.h"
#include "../Device/Time.h"
#include "../Math/Math.h"
#include <rapidjson/document.h>
#include <memory>
#include <vector>

class GameObject;
struct Material;

class FriedChickenComponent : public Component {
    enum class Surface {
        RIGHT,
        FORE,
        LEFT,
        UP,
        BOTTOM,
        BACK,

        NUM_SURFACE
    };

    enum class State {
        FRY,
        FALL,
        WAITING_COLLECTION,
        EATEN
    };

public:
    FriedChickenComponent(std::shared_ptr<GameObject> owner);
    ~FriedChickenComponent();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(DebugInfoList* inspect) const override;
    //マネージャーから必要な値をもらう
    void firstSet(float good);
    //揚げ直す前の状態に戻す
    void initialize();
    //揚げる面を変更
    void changeSurface();
    //強制的に揚げ終わる
    void finishFryed();
    //食われる
    void eaten();
    //面の数の取得
    int getNumSurface() const;
    //揚げ終わるまでの時間比率の取得
    float getFryRate(int surfaceIndex) const;

    //揚げている途中か
    bool isFrying() const;
    //落ちている途中か
    bool isFalling() const;
    //揚げ終わっているか
    bool isFinished() const;
    //食われたか
    bool isEaten() const;

private:
    //下の面を揚げる
    void frying();
    //揚げ具合によって色を変える
    void changeFryedColor();
    Vector3 getChangeColor(Surface surface);
    //両面揚げれたか
    void successFrying();
    //空中から落下させる
    void fall();
    //油に浸かったら揚げ始める
    void soakedInOil();

private:
    State mState;
    std::vector<std::unique_ptr<Time>> mFryTimer;
    Vector2 mRandomRangePositionX;
    Vector2 mRandomRangePositionZ;
    Vector2 mRandomRangeScale;
    Vector3 mInitColor;
    Vector3 mFryedColor;
    Vector3 mBurntColor;

    std::shared_ptr<Material> mUpColor;
    std::shared_ptr<Material> mBottomColor;
    std::shared_ptr<Material> mLeftColor;
    std::shared_ptr<Material> mRightColor;
    std::shared_ptr<Material> mForeColor;
    std::shared_ptr<Material> mBackColor;

    float mGood;
    float mFallSpeed;
};

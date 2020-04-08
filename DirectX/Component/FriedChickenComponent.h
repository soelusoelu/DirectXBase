#pragma once

#include "Component.h"
#include "../Device/Time.h"
#include "../Math/Math.h"
#include <rapidjson/document.h>
#include <memory>
#include <vector>

class GameObject;
class ChickenMeshComponent;

class FriedChickenComponent : public Component {
    enum class Surface {
        UP,
        BOTTOM,

        NUM_SURFACE
    };

    enum class State {
        FRY,
        FALL,
        WAITING_COLLECTION
    };

public:
    FriedChickenComponent(std::shared_ptr<GameObject> owner);
    ~FriedChickenComponent();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(debugInfoList* inspect) const override;
    //揚げ直す前の状態に戻す
    void initialize();
    //揚げる面を変更
    void changeSurface();
    //強制的に揚げ終わる
    void finishFryed();

    //揚げている途中か
    bool isFrying() const;
    //落ちている途中か
    bool isFalling() const;
    //揚げ終わっているか
    bool isFinished() const;

private:
    //下の面を揚げる
    void frying();
    //揚げ具合によって色を変える
    void changeFryedColor();
    //両面揚げれたか
    void successFrying();
    //空中から落下させる
    void fall();
    //油に浸かったら揚げ始める
    void soakedInOil();

private:
    std::shared_ptr<ChickenMeshComponent> mMeshComp;
    Surface mSurface;
    State mState;
    std::vector<std::unique_ptr<Time>> mFryTimer;
    Vector2 mRandomRangePositionX;
    Vector2 mRandomRangePositionZ;
    Vector2 mRandomRangeScale;
    Vector3 mInitColor;
    Vector3 mFryedColor;
    float mFallSpeed;
};

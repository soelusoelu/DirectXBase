#pragma once

#include "Component.h"
#include "../Device/Time.h"
#include "../Math/Math.h"
#include <rapidjson/document.h>
#include <memory>
#include <vector>

class GameObject;
class MeshComponent;

class FriedChickenComponent : public Component {
    enum class Surface {
        FRONT,
        BACK,

        NUM_SURFACE
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
    //両面揚げれたか
    bool successFrying() const;

private:
    //下の面を揚げる
    void frying();
    //揚げ具合によって色を変える
    void changeFryedColor();

private:
    std::shared_ptr<MeshComponent> mMeshComp;
    Surface mState;
    std::vector<std::unique_ptr<Time>> mFryTimer;
    Vector2 mRandomRangePositionX;
    Vector2 mRandomRangePositionZ;
    Vector2 mRandomRangeScale;
    Vector3 mInitColor;
    Vector3 mFryedColor;
};

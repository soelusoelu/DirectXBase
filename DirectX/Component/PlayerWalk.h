#pragma once

#include "Component.h"
#include "IPlayerWalk.h"
#include "../Math/Math.h"
#include <memory>

class Camera;
enum class KeyCode;
enum class JoyCode;

class PlayerWalk : public Component, public IPlayerWalk {
public:
    PlayerWalk();
    ~PlayerWalk();
    virtual void start() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(DebugInfoList* inspect) const override;

    virtual const Vector3& getMoveDirection() const override;

    void walkUpdate();

private:
    std::shared_ptr<Camera> mCamera;
    Vector3 mMoveDir;
    KeyCode mRollKey;
    JoyCode mRollPad;
    float mMoveSpeed;
    float mMeshRadius;
};

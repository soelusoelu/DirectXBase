#pragma once

#include "Component.h"
#include "IPlayerWalk.h"
#include "../Math/Math.h"
#include <memory>

enum class KeyCode;
enum class JoyCode;

class PlayerWalk : public Component, public IPlayerWalk {
public:
    PlayerWalk(std::shared_ptr<GameObject> owner);
    ~PlayerWalk();
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(DebugInfoList* inspect) const override;

    virtual const Vector3& getMoveDirection() const override;

    void walkUpdate();

private:
    Vector3 mMoveDir;
    KeyCode mRollKey;
    JoyCode mRollPad;
    float mMoveSpeed;
};

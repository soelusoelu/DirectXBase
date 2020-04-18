#pragma once

#include "../Math/Math.h"

class IPlayerJump {
public:
    virtual ~IPlayerJump() = default;
    //ジャンプした瞬間か
    virtual bool isJumpStart() const = 0;
    //ジャンプ中か
    virtual bool isJumping() const = 0;
    //着地した瞬間か
    virtual bool isJumpEnd() const = 0;
    //ターゲットの位置をセット
    virtual void setTargetPosition(const Vector3& pos) = 0;
};

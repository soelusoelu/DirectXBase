#pragma once

#include "../Math/Math.h"
#include <functional>

class IPlayerJump {
public:
    virtual ~IPlayerJump() = default;
    //ジャンプ中か
    virtual bool isJumping() const = 0;
    //ターゲットの位置をセット
    virtual void setTargetPosition(const Vector3& pos) = 0;
    //ジャンプできるか
    virtual void canJump(bool value) = 0;
    //ジャンプした瞬間のコールバック登録
    virtual void addJumpStartObserver(const std::function<void()>& f) = 0;
    //着地した瞬間のコールバック登録
    virtual void addJumpEndObserver(const std::function<void()>& f) = 0;
};

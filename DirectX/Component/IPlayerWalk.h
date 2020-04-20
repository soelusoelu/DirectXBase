#pragma once

#include "../Math/Math.h"

class IPlayerWalk {
public:
    virtual ~IPlayerWalk() = default;
    //移動する方向を取得
    virtual const Vector3& getMoveDirection() const = 0;
};

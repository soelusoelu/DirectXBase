#pragma once

#include "ChickenSurface.h"
#include "FryState.h"

class IChickenFry {
public:
    virtual ~IChickenFry() = default;
    //初期化
    virtual void initialize() = 0;
    //毎フレーム更新
    virtual void update() = 0;
    //すべての面が焦げたか
    virtual bool isBurntAllSurfaces() const = 0;
    //焦げた面が一定秒数以上揚げられてるか
    virtual bool isTooBurnt() const = 0;
    //半分以上の面が揚がってるか
    virtual bool isBurntHalfSurfaces() const = 0;
    //指定した面の揚げ状態の取得
    virtual FryState getFryState(ChickenSurface surface) const = 0;
    virtual FryState getFryState(unsigned surfaceIndex) const = 0;
};
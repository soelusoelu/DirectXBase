﻿#pragma once

#include "Component.h"
#include "IPlayerJump.h"
#include "IPlayerWalk.h"
#include <functional>

class PlayerWalk;
class PlayerJump;

//プレイヤー固有のコンポーネントを管理するクラス
class PlayerComponent : public Component {
    enum class State {
        WALK,
        JUMP
    };

public:
    PlayerComponent();
    ~PlayerComponent();
    virtual void start() override;
    virtual void update() override;

    std::shared_ptr<IPlayerWalk> getWalkState() const;
    std::shared_ptr<IPlayerJump> getJumpState() const;
    bool isWalking() const;
    void onAwakenJump(const std::function<void()>& f);

private:
    std::shared_ptr<PlayerWalk> mWalk;
    std::shared_ptr<PlayerJump> mJump;
    std::function<void()> mAwakenJump;
    State mState;
    bool mIsWaitFirstFrame;
};

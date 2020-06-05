#pragma once

#include "Component.h"
#include "IPlayerJump.h"
#include "../Input/Input.h"
#include "../Math/Math.h"

class PlayerJump : public Component, public IPlayerJump {
    enum class State {
        JUMPING,
        NONE
    };

public:
    PlayerJump();
    ~PlayerJump();
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;

    virtual bool isJumpStart() const override;
    virtual bool isJumping() const override;
    virtual bool isJumpEnd() const override;
    virtual void setTargetPosition(const Vector3& pos) override;
    virtual void canJump(bool value) override;

    //状態を更新
    void stateUpdate();
    //ジャンプ実行
    void jump();
    //メインアップデート
    void jumpUpdate();

private:
    //ジャンプ時の移動
    void move();
    //ジャンプから着地したか
    void stop();
    //ジャンプ時の初期化
    void startInitialize();

private:
    State mCurrentState;
    State mPreviousState;
    Vector3 mJumpStartPosition;
    Vector3 mJumpTargetPosition;
    KeyCode mJumpKey;
    JoyCode mJumpPad;
    float mSpeed;
    float mMoveRate;
    bool mCanJump;
};

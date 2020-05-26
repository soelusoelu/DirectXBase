#pragma once

#include "Component.h"
#include "IPlayerJump.h"
#include "../Math/Math.h"

enum class KeyCode;
enum class JoyCode;

class PlayerJump : public Component, public IPlayerJump {
    enum class State {
        JUMPING,
        NONE
    };

public:
    PlayerJump();
    ~PlayerJump();
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;

    virtual bool isJumpStart() const override;
    virtual bool isJumping() const override;
    virtual bool isJumpEnd() const override;
    virtual void setTargetPosition(const Vector3& pos) override;
    virtual void canJump(bool value) override;

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

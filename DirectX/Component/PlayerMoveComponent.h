#pragma once

#include "Component.h"
#include "../Math/Math.h"
#include <memory>

class GameObject;
enum class KeyCode;
enum class JoyCode;

class PlayerMoveComponent : public Component {
    enum class State {
        WALK,
        JUMP
    };

public:
    PlayerMoveComponent(std::shared_ptr<GameObject> owner);
    ~PlayerMoveComponent();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(DebugInfoList* inspect) const override;
    //歩行中か
    bool isWalking() const;
    //ジャンプした瞬間か
    bool isJumpStart() const;
    //ジャンプ中か
    bool isJumping() const;
    //着地した瞬間か
    bool isJumpEnd() const;
    //ターゲットの位置をセット
    void setTargetPosition(const Vector3& pos);
    //移動する方向を取得
    const Vector3& getMoveDirection() const;

private:
    //ジャンプするか
    void jump();
    //移動
    void walk();
    //ジャンプ時の移動
    void jumpMove();
    //ジャンプから着地したか
    void jumpStop();
    //ジャンプ時の初期化
    void jumpStartInitialize();

private:
    State mState;
    Vector3 mJumpStartPosition;
    Vector3 mJumpTargetPosition;
    Vector3 mMoveDir;
    KeyCode mJumpKey;
    JoyCode mJumpPad;
    float mMoveSpeed;
    float mJumpMoveRate;
    bool mJumpStart;
    bool mJumpEnd;
    bool mIsWaitFirstFrame;
};

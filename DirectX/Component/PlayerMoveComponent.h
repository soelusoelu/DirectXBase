#pragma once

#include "Component.h"
#include "../Math/Math.h"
#include <memory>

class GameObject;
class MeshComponent;

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
    virtual void drawDebugInfo(debugInfoList* inspect) const override;
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

private:
    //ジャンプするか
    void jump();
    //移動
    void move();
    //ジャンプ時の移動
    void jumpMove();
    //ジャンプから着地したか
    void jumpStop();

private:
    std::shared_ptr<MeshComponent> mMesh;
    State mState;
    Vector3 mJumpTargetPosition;
    float mMoveSpeed;
    bool mJumpStart;
    bool mJumpEnd;
};

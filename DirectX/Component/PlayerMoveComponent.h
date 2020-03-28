#pragma once

#include "Component.h"
#include "../Math/Math.h"
#include <memory>

class Actor;
class MeshComponent;

class PlayerMoveComponent : public Component {
    enum class State {
        WALK,
        JUMP
    };

public:
    PlayerMoveComponent(std::shared_ptr<Actor> owner);
    ~PlayerMoveComponent();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(debugInfoList* inspect) const override;
    //ジャンプ中か
    bool isJump() const;
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
};

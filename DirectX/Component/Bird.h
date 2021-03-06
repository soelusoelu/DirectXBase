﻿#pragma once

#include "Component.h"
#include <memory>

class BirdOrbit;
class Time;
class MeshComponent;
class GameObject;
class SphereCollisionComponent;
class SoundComponent;

class Bird : public Component {
    enum class State {
        WAIT,
        PREDICT_LINE,
        MOVE,
        HIT_MOVE
    };

public:
    Bird();
    ~Bird();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;

private:
    void waiting();
    void predictLine();
    void move();
    void updateCollider();
    void hitMove();
    void takeChicken();
    void isEndMoving();
    void initialize();
    void finalize();

private:
    std::shared_ptr<BirdOrbit> mOrbit;
    std::shared_ptr<MeshComponent> mMesh;
    std::shared_ptr<SphereCollisionComponent> mCollider;
    std::shared_ptr<SoundComponent> mSound;
    std::unique_ptr<Time> mRestartTimer;
    std::shared_ptr<GameObject> mTarget;
    State mState;
    float mMoveSpeed;
    float mClimbSpeed;
    float mCollideRadius;
};

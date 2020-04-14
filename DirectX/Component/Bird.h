#pragma once

#include "Component.h"
#include <memory>

class Timer;
class MeshComponent;
class GameObject;

class Bird : public Component {
    enum class State {
        WAIT,
        MOVE
    };

public:
    Bird(std::shared_ptr<GameObject> owner);
    ~Bird();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(DebugInfoList* inspect) const override;

private:
    void move();
    void takeChicken();
    void isEndMoving();
    void initialize();

private:
    std::shared_ptr<Timer> mTimer;
    std::shared_ptr<MeshComponent> mMesh;
    std::shared_ptr<GameObject> mTarget;
    State mState;
    float mMoveSpeed;
};

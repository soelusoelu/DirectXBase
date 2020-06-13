#pragma once

#include "Component.h"
#include "../Math/Math.h"
#include <memory>

class Sprite3D;
class Time;
class PlayerComponent;
class PlayerChickenConnection;

class OneRemain : public Component {
public:
    OneRemain();
    ~OneRemain();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    void addObserver(const std::shared_ptr<PlayerComponent>& player, const std::shared_ptr<PlayerChickenConnection>& pcc);
    void setPositionAndRadius(const Vector3& pos, float radius);

private:
    std::shared_ptr<Sprite3D> mSprite;
    std::unique_ptr<Time> mHiddenTimer;
    Vector3 mOffset;
};

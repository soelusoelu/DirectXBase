#pragma once

#include "Component.h"
#include <memory>

class Sprite3D;

class BirdOrbit : public Component {
public:
    BirdOrbit();
    ~BirdOrbit();
    virtual void start() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;
    void setActive(bool value);
    void setPositionZ(float z);
    float getTime() const;

private:
    std::shared_ptr<Sprite3D> mSprite;
    float mTimer;
};

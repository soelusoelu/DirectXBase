#pragma once

#include "Component.h"

class ChickenRise : public Component {
public:
    ChickenRise();
    ~ChickenRise();
    virtual void start() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;
    void initialize();
    void rise();
    bool isEnd();

private:
    float mUpSpeed;
    float mCurrentRotation;
    float mRotateSpeed;
    float mRotateRange;
    float mDefaultRotateRange;
    float mRotateRateOfUp;
};

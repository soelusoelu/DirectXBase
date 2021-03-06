﻿#pragma once

#include "Component.h"

class Field : public Component {
public:
    Field();
    ~Field();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;

private:
    bool mIsRoll;
    float mRollSpeed;
};

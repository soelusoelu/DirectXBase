#pragma once

#include "Component.h"
#include "../Utility/Math.h"

class Actor;

class PointLightComponent : public Component {
public:
    PointLightComponent(Actor* owner);
    ~PointLightComponent();
    virtual void start() override;
    virtual void update() override;

    Vector3 mDiffuseColor; //F
    float mInnerRadius; //‚±‚Ì”¼ŒaˆÈ“à‚¾‚ÆŠ®‘S‚È‹P“x‚ÅÆ‚ç‚·
    float mOuterRadius; //Œõ‚Ì“Í‚­”¼Œa
};

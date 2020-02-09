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

    Vector3 mDiffuseColor; //�F
    float mInnerRadius; //���̔��a�ȓ����Ɗ��S�ȋP�x�ŏƂ炷
    float mOuterRadius; //���̓͂����a
};

#pragma once

#include "Component.h"

class Field : public Component {
public:
    Field(std::shared_ptr<GameObject> owner);
    ~Field();
    virtual void start() override;
    virtual void update() override;
};

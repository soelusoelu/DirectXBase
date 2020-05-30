#pragma once

#include "../Component.h"
#include <memory>

class Scene;

class OperationExplanation : public Component {
public:
    OperationExplanation();
    ~OperationExplanation();
    virtual void start() override;
    virtual void update() override;

private:
    std::shared_ptr<Scene> mScene;
    bool mIsEnd;
};

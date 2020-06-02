#pragma once

#include "../Component.h"
#include <memory>

class Scene;
class ResultChicken;

class ResultScene : public Component {
public:
    ResultScene();
    ~ResultScene();
    virtual void start() override;
    virtual void update() override;

private:
    std::shared_ptr<Scene> mScene;
    std::shared_ptr<ResultChicken> mResult;
    int mScore;
};

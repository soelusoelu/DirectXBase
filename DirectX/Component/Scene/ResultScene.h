#pragma once

#include "../Component.h"
#include <memory>

class Scene;
class ResultChicken;
enum class KeyCode;
enum class JoyCode;

class ResultScene : public Component {
public:
    ResultScene();
    ~ResultScene();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;

private:
    std::shared_ptr<Scene> mScene;
    std::shared_ptr<ResultChicken> mResult;
    KeyCode mEnterKey;
    JoyCode mEnterPad;
    int mScore;
};

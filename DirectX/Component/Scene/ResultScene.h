#pragma once

#include "../Component.h"
#include <memory>

class Scene;
enum class KeyCode;
enum class JoyCode;

class ResultScene : public Component {
public:
    ResultScene(const std::shared_ptr<GameObject>& owner);
    ~ResultScene();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;

private:
    std::shared_ptr<Scene> mScene;
    KeyCode mEnterKey;
    JoyCode mEnterPad;
    int mScore;
};

#pragma once

#include "../Component.h"
#include <memory>

class Scene;
enum class KeyCode;
enum class JoyCode;

class Title : public Component {
public:
    Title(const std::shared_ptr<GameObject>& owner);
    ~Title();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;

private:
    std::shared_ptr<Scene> mScene;
    KeyCode mEnterKey;
    JoyCode mEnterPad;
    bool mEndFrame;
};

#pragma once

#include "../Component.h"
#include <memory>

class Scene;
class SpriteComponent;

class Title : public Component {
    enum class Sprites {
        TITLE,
        PRESS_A
    };

    enum class Sounds {
        TITLE,
        HEY
    };

public:
    Title();
    ~Title();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;

private:
    void fadePressA();

private:
    std::shared_ptr<Scene> mScene;
    std::shared_ptr<SpriteComponent> mPressA;
    float mCurrentFadeValue;
    float mFadeSpeed;
};

#pragma once

#include "Component.h"
#include "../Math/Math.h"
#include <rapidjson/document.h>

class GameObject;

class Score : public Component {
public:
    Score(std::shared_ptr<GameObject> owner);
    ~Score();
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const override;
    void addScore(int score);
    int getScore() const;

private:
    Vector2 mDrawPosition;
    Vector2 mDrawScale;
    int mScore;
    int mHighScore;
};

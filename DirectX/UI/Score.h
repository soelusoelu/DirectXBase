#pragma once

#include "UI.h"
#include "../Utility/Math.h"
#include <rapidjson/document.h>

class Score : public UI {
public:
    Score(std::shared_ptr<Renderer> renderer);
    ~Score();
    virtual void updateUI() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const override;

private:
    Vector2 mDrawPosition;
    Vector2 mDrawScale;
    int mScore;
    int mHighScore;
};

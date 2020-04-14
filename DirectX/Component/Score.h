#pragma once

#include "Component.h"

class Text;

class Score : public Component {
public:
    Score(std::shared_ptr<GameObject> owner);
    ~Score();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const override;
    void addScore(int score);
    int getScore() const;

private:
    std::shared_ptr<Text> mText;
    int mScore;
    int mHighScore;
};

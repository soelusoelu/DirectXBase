#pragma once

#include "Component.h"

class TextNumber;

class Score : public Component {
public:
    Score();
    ~Score();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value* inObj) const override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;
    void addScore(int score);
    int getScore() const;

private:
    std::shared_ptr<TextNumber> mText;
    int mScore;
    int mHighScore;
};

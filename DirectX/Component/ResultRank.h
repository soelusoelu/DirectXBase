#pragma once

#include "Component.h"
#include "Rank.h"
#include <memory>
#include <string>

class SpriteComponent;

class ResultRank : public Component {
public:
    ResultRank();
    ~ResultRank();
    virtual void start() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;
    void initialize(int score);
    Rank getRank() const;
    void setActive(bool value);

private:
    void selectRank(int score);
    void selectTexture();

private:
    std::shared_ptr<SpriteComponent> mSprite;
    Rank mRank;
    int mLow;
    int mMiddle;
    int mHigh;
    std::string mLowTextureName;
    std::string mMiddleTextureName;
    std::string mHighTextureName;
    std::string mSuperTextureName;
};

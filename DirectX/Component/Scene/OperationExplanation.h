#pragma once

#include "../Component.h"
#include <memory>
#include <string>
#include <vector>

class Scene;
class SpriteComponent;

class OperationExplanation : public Component {
public:
    OperationExplanation();
    ~OperationExplanation();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;

private:
    //クリックするごとに操作説明の画像を更新
    void nextSprite();

private:
    std::shared_ptr<Scene> mScene;
    std::shared_ptr<SpriteComponent> mSprite;
    std::vector<std::string> mOperationTextures;
    unsigned mCurrentIndex;
    bool mIsEnd;
};

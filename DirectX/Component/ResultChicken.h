#pragma once

#include "Component.h"
#include <memory>

class GameObject;
class ResultRank;
class ResultChickenManager;
class Fade;
class Time;
class Text;

class ResultChicken : public Component {
    enum class State {
        FALL,
        FADE,
        RESULT
    };

public:
    ResultChicken();
    ~ResultChicken();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    void initialize(int score);
    bool isResult() const;

private:
    void fallUpdate();
    void fadeUpdate();
    void skipPerform();

private:
    std::shared_ptr<ResultRank> mRank;
    std::shared_ptr<GameObject> mResultChickenManager;
    std::shared_ptr<Fade> mFade;
    std::unique_ptr<Time> mFallToFadeTimer;
    std::shared_ptr<Text> mText;
    State mState;
    int mScore;
};

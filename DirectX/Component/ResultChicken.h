#pragma once

#include "Component.h"
#include <memory>

class ResultRank;
class Time;

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
    virtual void initialize(int score);

private:
    std::shared_ptr<ResultRank> mRank;
    std::unique_ptr<Time> mFallToFadeTimer;
    State mState;
    int mScore;
};

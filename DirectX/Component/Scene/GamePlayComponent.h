#pragma once

#include "../Component.h"

class FriedChickenManager;
class PlayerChickenConnection;
class Score;
class Timer;
class JumpTarget;
class Oil;

class GamePlayComponent : public Component {
public:
    GamePlayComponent(const std::shared_ptr<GameObject>& owner);
    ~GamePlayComponent();
    virtual void awake() override;
    virtual void update() override;
    bool isEnd() const;

private:
    std::shared_ptr<FriedChickenManager> mFriedChickenManager;
    std::shared_ptr<PlayerChickenConnection> mPCConnection;
    std::shared_ptr<Score> mScore;
    std::shared_ptr<Timer> mTimeLimitTimer;
    std::shared_ptr<JumpTarget> mJumpTarget;
    std::shared_ptr<Oil> mOil;
    bool mIsEnd;
    bool mIsFirstSleep;
};

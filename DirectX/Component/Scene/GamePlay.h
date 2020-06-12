#pragma once

#include "../Component.h"
#include <memory>

class Scene;
class FriedChickenManager;
class PlayerChickenConnection;
class Score;
class TimeLimit;
class JumpTarget;
class Oil;
class OneRemain;

class GamePlay : public Component {
public:
    GamePlay();
    ~GamePlay();
    virtual void awake() override;
    virtual void start() override;
    virtual void update() override;

private:
    std::shared_ptr<Scene> mScene;
    std::shared_ptr<FriedChickenManager> mFriedChickenManager;
    std::shared_ptr<PlayerChickenConnection> mPCConnection;
    std::shared_ptr<Score> mScore;
    std::shared_ptr<TimeLimit> mTimeLimit;
    std::shared_ptr<JumpTarget> mJumpTarget;
    std::shared_ptr<Oil> mOil;
    std::shared_ptr<OneRemain> mOneRemain;
    bool mIsFirstSleep;
};

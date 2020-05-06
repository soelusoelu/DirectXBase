#pragma once

#include "SceneBase.h"
#include <memory>

class FriedChickenManager;
class PlayerChickenConnection;
class Score;
class Timer;
class JumpTarget;
class Oil;
class Sound;

class GamePlay : public SceneBase {
    enum class State {
        PLAY,
        PAUSE,
    };

public:
    GamePlay();
    ~GamePlay();
    virtual void start() override;
    virtual void update() override;

private:
    std::shared_ptr<FriedChickenManager> mFriedChickenManager;
    std::shared_ptr<PlayerChickenConnection> mPCConnection;
    std::shared_ptr<Score> mScore;
    std::shared_ptr<Timer> mTimeLimitTimer;
    std::shared_ptr<JumpTarget> mJumpTarget;
    std::shared_ptr<Oil> mOil;
    std::shared_ptr<Sound> mBGM;
    State mState;
};

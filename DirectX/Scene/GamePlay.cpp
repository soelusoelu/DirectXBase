#include "GamePlay.h"
#include "../Actor/PlayerActor.h"
#include "../Component/Collider.h"
#include "../Device/Physics.h"
#include "../System/Game.h"
#include "../UI/Pause.h"

GamePlay::GamePlay(std::shared_ptr<Renderer> renderer) :
    SceneBase(renderer),
    mPhysics(new Physics()),
    mState(GameState::PLAY),
    mPauseKey(KeyCode::Alpha1)
{
    Collider::setPhysics(mPhysics);
    new PlayerActor(mRenderer);
}

GamePlay::~GamePlay() {
    SAFE_DELETE(mPhysics);
    //なぜかColliderのPhysicsが生きてるから
    Collider::setPhysics(nullptr);
}

void GamePlay::updateScene() {
    if (mState == GameState::PLAY) {
        //総当たり判定
        mPhysics->sweepAndPrune();

        if (Input::getKeyDown(mPauseKey)) {
            new Pause(shared_from_this());
        }
    } else if (mState == GameState::PAUSED) {

    }
}

GameState GamePlay::getState() const {
    return mState;
}

void GamePlay::setState(GameState state) {
    mState = state;
}

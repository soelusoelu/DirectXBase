#include "GamePlay.h"
#include "../Actor/Actor.h"
#include "../Actor/ActorManager.h"
#include "../Actor/PlayerActor.h"
#include "../Component/Collider.h"
#include "../Device/Physics.h"
#include "../System/Game.h"
#include "../UI/Pause.h"

GamePlay::GamePlay() :
    SceneBase(),
    mActorManager(new ActorManager()),
    mPhysics(new Physics()),
    mState(GameState::PLAY),
    mPauseKey(KeyCode::Alpha1) {
    Actor::setActorManager(mActorManager);
    Collider::setPhysics(mPhysics);
}

GamePlay::~GamePlay() {
    SAFE_DELETE(mActorManager);
    SAFE_DELETE(mPhysics);
    //�Ȃ���Collider��Physics�������Ă邩��
    Collider::setPhysics(nullptr);
}

void GamePlay::startScene() {
    new PlayerActor(mRenderer);
}

void GamePlay::updateScene() {
    if (mState == GameState::PLAY) {
        //���A�N�^�[�A�b�v�f�[�g
        mActorManager->update();
        //�������蔻��
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

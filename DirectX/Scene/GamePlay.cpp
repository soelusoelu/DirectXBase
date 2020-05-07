#include "GamePlay.h"
#include "ResultScene.h"
#include "../Component/ComponentManager.h"
#include "../Component/Scene/GamePlayComponent.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectFactory.h"
#include "../Input/Input.h"
#include "../Input/Keyboard.h"
#include "../System/Game.h"

GamePlay::GamePlay() :
    SceneBase(),
    mGamePlay(nullptr),
    mState(State::PLAY) {
}

GamePlay::~GamePlay() = default;

void GamePlay::start() {
    auto gamePlay = GameObjectCreater::create("GamePlay");
    mGamePlay = gamePlay->componentManager()->getComponent<GamePlayComponent>();
}

void GamePlay::update() {
    if (mState == State::PLAY) {
        if (mGamePlay->isEnd()) {
            nextScene(std::make_shared<ResultScene>(0));
            //nextScene(std::make_shared<ResultScene>(mScore->getScore()));
        }
    } else if (mState == State::PAUSE) {
    }

    if (Input::keyboard()->getKeyDown(KeyCode::Escape)) {
        Game::quit();
    }
    //リセット
#ifdef _DEBUG
    if (Input::keyboard()->getKeyDown(KeyCode::R)) {
        nextScene(std::make_shared<GamePlay>());
    }
#endif // _DEBUG
}

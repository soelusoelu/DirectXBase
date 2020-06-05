#include "ResultScene.h"
#include "Scene.h"
#include "../ComponentManager.h"
#include "../ResultChicken.h"
#include "../Score.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectFactory.h"
#include "../../GameObject/GameObjectManager.h"
#include "../../Input/Input.h"

ResultScene::ResultScene() :
    Component(),
    mScene(nullptr),
    mResult(nullptr),
    mScore(0) {
}

ResultScene::~ResultScene() = default;

void ResultScene::start() {
    mScene = owner()->componentManager()->getComponent<Scene>();

    auto scoreObj = owner()->getGameObjectManager()->find("Score");
    auto score = scoreObj->componentManager()->getComponent<Score>();

    auto resultChicken = GameObjectCreater::create("ResultChicken");
    mResult = resultChicken->componentManager()->getComponent<ResultChicken>();
    mResult->initialize(score->getScore());

    scoreObj->destroy();
}

void ResultScene::update() {
    if (!mResult->isResult()) {
        return;
    }

    auto isEnd = Input::joyPad()->getEnter();
#ifdef _DEBUG
    if (!isEnd) {
        isEnd = Input::keyboard()->getEnter();
    }
#endif // _DEBUG

    if (isEnd) {
        mScene->next("Title");
    }
}

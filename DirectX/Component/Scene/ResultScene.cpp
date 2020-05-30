#include "ResultScene.h"
#include "Scene.h"
#include "../ComponentManager.h"
#include "../Result.h"
#include "../ResultChicken.h"
#include "../Score.h"
#include "../SoundComponent.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectFactory.h"
#include "../../GameObject/GameObjectManager.h"
#include "../../Input/Input.h"
#include "../../Input/JoyPad.h"
#include "../../Input/Keyboard.h"
#include "../../Utility/LevelLoader.h"
#include <string>

ResultScene::ResultScene() :
    Component(),
    mScene(nullptr),
    mEnterKey(KeyCode::None),
    mEnterPad(JoyCode::None),
    mScore(0) {
}

ResultScene::~ResultScene() = default;

void ResultScene::start() {
    mScene = owner()->componentManager()->getComponent<Scene>();
    auto scoreObj = owner()->getGameObjectManager()->find("Score");
    auto score = scoreObj->componentManager()->getComponent<Score>();

    auto resultChicken = GameObjectCreater::create("ResultChicken");
    auto resChi = resultChicken->componentManager()->getComponent<ResultChicken>();
    resChi->initialize(score->getScore());
    auto rcm = GameObjectCreater::create("ResultChickenManager");

    //auto resultObj = GameObjectCreater::createUI("Result");
    //auto result = resultObj->componentManager()->getComponent<Result>();
    //result->setScore(score->getScore());

    //owner()->componentManager()->getComponents<SoundComponent>().back()->playSE();

    scoreObj->destroy();
}

void ResultScene::update() {
    auto isEnd = Input::joyPad()->getJoyDown(mEnterPad);
#ifdef _DEBUG
    if (!isEnd) {
        isEnd = Input::keyboard()->getKeyDown(mEnterKey);
    }
#endif // _DEBUG

    if (isEnd) {
        mScene->next("Title");
    }
}

void ResultScene::loadProperties(const rapidjson::Value& inObj) {
    std::string src;
    if (JsonHelper::getString(inObj, "enterKey", &src)) {
        Keyboard::stringToKeyCode(src, &mEnterKey);
    }
    if (JsonHelper::getString(inObj, "enterPad", &src)) {
        JoyPad::stringToJoyCode(src, &mEnterPad);
    }
}

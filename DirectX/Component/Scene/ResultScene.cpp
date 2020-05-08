#include "ResultScene.h"
#include "Scene.h"
#include "../../Component/ComponentManager.h"
#include "../../Component/Result.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/GameObjectFactory.h"
#include "../../Input/Input.h"
#include "../../Input/JoyPad.h"
#include "../../Input/Keyboard.h"
#include "../../Utility/LevelLoader.h"
#include <string>

ResultScene::ResultScene(const std::shared_ptr<GameObject>& owner) :
    Component(owner, "ResultScene"),
    mScene(nullptr),
    mEnterKey(KeyCode::None),
    mEnterPad(JoyCode::None),
    mScore(0) {
}

ResultScene::~ResultScene() = default;

void ResultScene::start() {
    mScene = owner()->componentManager()->getComponent<Scene>();

    auto resultObj = GameObjectCreater::createUI("Result");
    auto result = resultObj->componentManager()->getComponent<Result>();
    result->setScore(mScore);
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
    Component::loadProperties(inObj);

    std::string src;
    if (JsonHelper::getString(inObj, "enterKey", &src)) {
        Keyboard::stringToKeyCode(src, &mEnterKey);
    }
    if (JsonHelper::getString(inObj, "enterPad", &src)) {
        JoyPad::stringToJoyCode(src, &mEnterPad);
    }
}

void ResultScene::setScore(int score) {
    mScore = score;
}

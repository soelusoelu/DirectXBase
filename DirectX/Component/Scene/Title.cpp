#include "Title.h"
#include "Scene.h"
#include "../ComponentManager.h"
#include "../../GameObject/GameObject.h"
#include "../../Input/Input.h"
#include "../../Input/JoyPad.h"
#include "../../Input/Keyboard.h"
#include "../../Utility/LevelLoader.h"
#include <string>

Title::Title(const std::shared_ptr<GameObject>& owner) :
    Component(owner, "Title"),
    mScene(nullptr),
    mEnterKey(KeyCode::None),
    mEnterPad(JoyCode::None) {
}

Title::~Title() = default;

void Title::start() {
    mScene = owner()->componentManager()->getComponent<Scene>();
}

void Title::update() {
    auto isEnd = Input::joyPad()->getJoyDown(mEnterPad);
#ifdef _DEBUG
    if (!isEnd) {
        isEnd = Input::keyboard()->getKeyDown(mEnterKey);
    }
#endif // _DEBUG

    if (isEnd) {
        mScene->next("GamePlay");
    }
}

void Title::loadProperties(const rapidjson::Value& inObj) {
    Component::loadProperties(inObj);

    std::string src;
    if (JsonHelper::getString(inObj, "enterKey", &src)) {
        Keyboard::stringToKeyCode(src, &mEnterKey);
    }
    if (JsonHelper::getString(inObj, "enterPad", &src)) {
        JoyPad::stringToJoyCode(src, &mEnterPad);
    }
}

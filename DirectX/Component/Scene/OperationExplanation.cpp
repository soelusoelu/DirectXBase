#include "OperationExplanation.h"
#include "Scene.h"
#include "../ComponentManager.h"
#include "../SpriteComponent.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/Transform2D.h"
#include "../../Input/Input.h"
#include "../../Input/JoyPad.h"
#include "../../Input/Keyboard.h"
#include "../../Utility/LevelLoader.h"
#include <string>

OperationExplanation::OperationExplanation() :
    Component(),
    mScene(nullptr),
    mEnterKey(KeyCode::None),
    mEnterPad(JoyCode::None),
    mIsEnd(false) {
}

OperationExplanation::~OperationExplanation() = default;

void OperationExplanation::start() {
    mScene = owner()->componentManager()->getComponent<Scene>();
}

void OperationExplanation::update() {
    auto isEnd = Input::joyPad()->getJoyDown(mEnterPad);
#ifdef _DEBUG
    if (!isEnd) {
        isEnd = Input::keyboard()->getKeyDown(mEnterKey);
    }
#endif // _DEBUG

    if (mIsEnd) {
        mScene->next("GamePlay");
    }
    if (isEnd) {
        auto sprites = owner()->componentManager()->getComponents<SpriteComponent>();
        sprites.back()->setActive(true);
        mIsEnd = true;
    }
}

void OperationExplanation::loadProperties(const rapidjson::Value & inObj) {
    Component::loadProperties(inObj);

    std::string src;
    if (JsonHelper::getString(inObj, "enterKey", &src)) {
        Keyboard::stringToKeyCode(src, &mEnterKey);
    }
    if (JsonHelper::getString(inObj, "enterPad", &src)) {
        JoyPad::stringToJoyCode(src, &mEnterPad);
    }
}

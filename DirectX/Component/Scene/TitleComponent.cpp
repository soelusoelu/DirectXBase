#include "TitleComponent.h"
#include "../../Input/Input.h"
#include "../../Input/JoyPad.h"
#include "../../Input/Keyboard.h"
#include "../../Utility/LevelLoader.h"
#include <string>

TitleComponent::TitleComponent(const std::shared_ptr<GameObject>& owner) :
    Component(owner, "TitleComponent"),
    mEnterKey(KeyCode::None),
    mEnterPad(JoyCode::None),
    mIsEnd(false) {
}

TitleComponent::~TitleComponent() = default;

void TitleComponent::update() {
    mIsEnd = Input::joyPad()->getJoyDown(mEnterPad);
#ifdef _DEBUG
    if (!mIsEnd) {
        mIsEnd = Input::keyboard()->getKeyDown(mEnterKey);
    }
#endif // _DEBUG
}

void TitleComponent::loadProperties(const rapidjson::Value& inObj) {
    Component::loadProperties(inObj);

    std::string src;
    if (JsonHelper::getString(inObj, "enterKey", &src)) {
        Keyboard::stringToKeyCode(src, &mEnterKey);
    }
    if (JsonHelper::getString(inObj, "enterPad", &src)) {
        JoyPad::stringToJoyCode(src, &mEnterPad);
    }
}

bool TitleComponent::isEnd() const {
    return mIsEnd;
}

#include "Title.h"
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

Title::Title(const std::shared_ptr<GameObject>& owner) :
    Component(owner, "Title"),
    mScene(nullptr),
    mEnterKey(KeyCode::None),
    mEnterPad(JoyCode::None),
    mEndFrame(false) {
}

Title::~Title() = default;

void Title::start() {
    mScene = owner()->componentManager()->getComponent<Scene>();

    auto sprites = owner()->componentManager()->getComponents<SpriteComponent>();
    sprites.back()->transform()->setPivot(Pivot::RIGHT_BOTTOM);
    sprites.back()->transform()->setPosition(Vector2(1920.f, 1080.f));
    sprites.back()->setActive(false);
}

void Title::update() {
    auto isEnd = Input::joyPad()->getJoyDown(mEnterPad);
#ifdef _DEBUG
    if (!isEnd) {
        isEnd = Input::keyboard()->getKeyDown(mEnterKey);
    }
#endif // _DEBUG

    if (mEndFrame) {
        mScene->next("GamePlay");
    }
    if (isEnd) {
        auto sprites = owner()->componentManager()->getComponents<SpriteComponent>();
        sprites.back()->setActive(true);

        mEndFrame = true;
    }
}

void Title::loadProperties(const rapidjson::Value & inObj) {
    Component::loadProperties(inObj);

    std::string src;
    if (JsonHelper::getString(inObj, "enterKey", &src)) {
        Keyboard::stringToKeyCode(src, &mEnterKey);
    }
    if (JsonHelper::getString(inObj, "enterPad", &src)) {
        JoyPad::stringToJoyCode(src, &mEnterPad);
    }
}

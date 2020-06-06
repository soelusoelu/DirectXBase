#include "Title.h"
#include "Scene.h"
#include "../ComponentManager.h"
#include "../SoundComponent.h"
#include "../SpriteComponent.h"
#include "../../Device/Time.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/Transform2D.h"
#include "../../Input/Input.h"
#include "../../Math/Math.h"
#include "../../Utility/LevelLoader.h"

Title::Title() :
    Component(),
    mScene(nullptr),
    mPressA(nullptr),
    mCurrentFadeValue(0.f),
    mFadeSpeed(0.f) {
}

Title::~Title() = default;

void Title::start() {
    mScene = owner()->componentManager()->getComponent<Scene>();
    auto sprites = owner()->componentManager()->getComponents<SpriteComponent>();
    mPressA = sprites[static_cast<unsigned>(Sprites::PRESS_A)];
}

void Title::update() {
    fadePressA();

    auto isEnd = Input::joyPad()->getEnter();
#ifdef _DEBUG
    if (!isEnd) {
        isEnd = Input::keyboard()->getEnter();
    }
#endif // _DEBUG

    if (isEnd) {
        auto sounds = owner()->componentManager()->getComponents<SoundComponent>();
        sounds[static_cast<unsigned>(Sounds::HEY)]->playSE();
        mScene->next("OperationExplanation");
    }
}

void Title::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getFloat(inObj, "fadeSpeed", &mFadeSpeed);
}

void Title::fadePressA() {
    mCurrentFadeValue += Time::deltaTime * mFadeSpeed;
    auto value = Math::abs(Math::sin(mCurrentFadeValue));
    mPressA->setAlpha(value);
}

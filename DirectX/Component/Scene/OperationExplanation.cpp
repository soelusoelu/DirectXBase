#include "OperationExplanation.h"
#include "Scene.h"
#include "../ComponentManager.h"
#include "../SpriteComponent.h"
#include "../../GameObject/GameObject.h"
#include "../../GameObject/Transform2D.h"
#include "../../Input/Input.h"
#include "../../Input/JoyPad.h"
#include "../../Input/Keyboard.h"
#include "../../System/Window.h"
#include "../../Utility/LevelLoader.h"

OperationExplanation::OperationExplanation() :
    Component(),
    mScene(nullptr),
    mCurrentIndex(0),
    mIsEnd(false) {
}

OperationExplanation::~OperationExplanation() = default;

void OperationExplanation::start() {
    mScene = owner()->componentManager()->getComponent<Scene>();
    mSprite = owner()->componentManager()->getComponent<SpriteComponent>();
    mSprite->setSprite(mOperationTextures[mCurrentIndex]);
    //操作説明の画像が、ウィンドウサイズと違ってたから修正してる
    mSprite->transform()->setScale(Vector2(
        1.f,
        static_cast<float>(Window::standardHeight()) / mSprite->getTextureSize().y
    ));
}

void OperationExplanation::update() {
    if (mIsEnd) {
        mScene->next("GamePlay");
    }
    nextSprite();
}

void OperationExplanation::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getStringArray(inObj, "operationTextures", &mOperationTextures);
}

void OperationExplanation::nextSprite() {
    auto isClick = Input::joyPad()->getEnter();
#ifdef _DEBUG
    if (!isClick) {
        isClick = Input::keyboard()->getEnter();
    }
#endif // _DEBUG

    if (!isClick) {
        return;
    }

    mCurrentIndex++;
    if (mCurrentIndex < mOperationTextures.size()) {
        mSprite->changeTexture(mOperationTextures[mCurrentIndex]);
    } else {
        auto sprites = owner()->componentManager()->getComponents<SpriteComponent>();
        sprites.back()->setActive(true);
        mIsEnd = true;
    }
}

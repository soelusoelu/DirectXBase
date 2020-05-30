#include "OperationExplanation.h"
#include "Scene.h"
#include "../ComponentManager.h"
#include "../SpriteComponent.h"
#include "../../GameObject/GameObject.h"
#include "../../Input/Input.h"
#include "../../Input/JoyPad.h"
#include "../../Input/Keyboard.h"

OperationExplanation::OperationExplanation() :
    Component(),
    mScene(nullptr),
    mIsEnd(false) {
}

OperationExplanation::~OperationExplanation() = default;

void OperationExplanation::start() {
    mScene = owner()->componentManager()->getComponent<Scene>();
}

void OperationExplanation::update() {
    auto isEnd = Input::joyPad()->getEnter();
#ifdef _DEBUG
    if (!isEnd) {
        isEnd = Input::keyboard()->getEnter();
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

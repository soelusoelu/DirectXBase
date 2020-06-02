#include "Title.h"
#include "Scene.h"
#include "../ComponentManager.h"
#include "../SoundComponent.h"
#include "../../GameObject/GameObject.h"
#include "../../Input/Input.h"
#include "../../Input/JoyPad.h"
#include "../../Input/Keyboard.h"
#include <string>

Title::Title() :
    Component(),
    mScene(nullptr) {
}

Title::~Title() = default;

void Title::start() {
    mScene = owner()->componentManager()->getComponent<Scene>();
}

void Title::update() {
    auto isEnd = Input::joyPad()->getEnter();
#ifdef _DEBUG
    if (!isEnd) {
        isEnd = Input::keyboard()->getEnter();
    }
#endif // _DEBUG

    if (isEnd) {
        owner()->componentManager()->getComponents<SoundComponent>().back()->playSE();
        mScene->next("OperationExplanation");
    }
}

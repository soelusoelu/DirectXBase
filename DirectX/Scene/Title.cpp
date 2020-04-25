#include "Title.h"
#include "GamePlay.h"
#include "../Sprite/Sprite.h"
#include "../Input/Input.h"
#include "../Input/JoyPad.h"
#include "../Input/Keyboard.h"

Title::Title() :
    SceneBase() {
}

Title::~Title() = default;

void Title::start() {
    auto title = std::make_shared<Sprite>("12Title.png");
    title->addToManager();
}

void Title::update() {
    if (Input::keyboard()->getKeyDown(KeyCode::Space) || Input::joyPad()->getJoyDown(JoyCode::A)) {
        nextScene(std::make_shared<GamePlay>());
    }
}

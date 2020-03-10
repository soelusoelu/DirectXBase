#include "Title.h"
#include "GamePlay.h"
#include "../Sprite/Sprite.h"
#include "../Utility/Input.h"
#include "../Utility/Keyboard.h"

Title::Title() :
    SceneBase() {
}

Title::~Title() = default;

void Title::start() {
    auto title = std::make_shared<Sprite>(mRenderer, "12Title.png");
    title->addToManager();
}

void Title::update() {
    if (Input::keyboard()->getKeyDown(KeyCode::Space) || Input::getJoyDown(JoyCode::A)) {
        nextScene(std::make_shared<GamePlay>());
    }
}

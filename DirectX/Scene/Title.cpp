#include "Title.h"
#include "GamePlay.h"

Title::Title() :
    SceneBase(),
    mNextKey(KeyCode::Space) {
}

Title::~Title() = default;

void Title::startScene() {
}

void Title::updateScene() {
    if (Input::getKeyDown(mNextKey)) {
        next(std::make_shared<GamePlay>());
    }
}

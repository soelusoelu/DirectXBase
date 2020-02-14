#include "Title.h"
#include "GamePlay.h"
#include "../Actor/Transform2D.h"
#include "../Sprite/Sprite.h"
#include "../System/Game.h"

Title::Title() :
    SceneBase(),
    mNextKey(KeyCode::Space),
    mNextJoy(JoyCode::A) {
}

Title::~Title() = default;

void Title::start() {
    auto title = new Sprite(mRenderer, "12Title.png");
    title->transform()->setPosition(Vector2(Game::WINDOW_WIDTH / 2.f, Game::WINDOW_HEIGHT / 2.f));
}

void Title::update() {
    if (Input::getKeyDown(mNextKey) || Input::getJoyDown(mNextJoy)) {
        nextScene(std::make_shared<GamePlay>());
    }
}

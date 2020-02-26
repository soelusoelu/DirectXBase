#include "Pause.h"
#include "../Scene/GamePlay.h"
#include "../Sprite/Sprite.h"

Pause::Pause(std::shared_ptr<GamePlay> game, std::shared_ptr<Renderer> renderer) :
    UI("Pause"),
    mGame(game),
    mCloseKey(KeyCode::Alpha2) {
    game->setPause();
    addSprite(std::make_shared<Sprite>(renderer, "cat.png"));
}

Pause::~Pause() {
    if (auto g = mGame.lock()) {
        g->setPlay();
    }
}

void Pause::updateUI() {
    if (Input::getKeyDown(mCloseKey)) {
        close();
    }
}

#include "Pause.h"
#include "../Scene/GamePlay.h"
#include "../Sprite/Sprite.h"

Pause::Pause(std::shared_ptr<GamePlay> game, std::shared_ptr<Renderer> renderer) :
    UI(),
    mGame(game),
    mCloseKey(KeyCode::Alpha2) {
    if (auto g = mGame.lock()) {
        g->setState(GameState::PAUSED);
    }
    addSprite(new Sprite(renderer, "cat.png", 0.2f));
}

Pause::~Pause() {
    if (auto g = mGame.lock()) {
        g->setState(GameState::PLAY);
    }
}

void Pause::updateUI() {
    if (Input::getKeyDown(mCloseKey)) {
        close();
    }
}

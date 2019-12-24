#include "Pause.h"
#include "../Scene/GamePlay.h"

Pause::Pause(std::shared_ptr<GamePlay> game) :
    UI(),
    mGame(game),
    mCloseKey(KeyCode::Alpha2) {
    if (auto g = mGame.lock()) {
        g->setState(GameState::PAUSED);
    }
}

Pause::~Pause() {
    if (auto g = mGame.lock()) {
        g->setState(GameState::PLAY);
    }
}

void Pause::update() {
    if (Input::getKeyDown(mCloseKey)) {
        close();
    }
}

#include "Title.h"
#include "GamePlay.h"
#include "../Component/ComponentManager.h"
#include "../Component/Scene/TitleComponent.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectFactory.h"

Title::Title() :
    SceneBase(),
    mTitle(nullptr) {
}

Title::~Title() = default;

void Title::start() {
    auto title = GameObjectCreater::create("Title");
    mTitle = title->componentManager()->getComponent<TitleComponent>();
}

void Title::update() {
    if (mTitle->isEnd()) {
        nextScene(std::make_shared<GamePlay>());
    }
}

#include "ResultScene.h"
#include "Title.h"
#include "../Component/ComponentManager.h"
#include "../Component/Result.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectFactory.h"
#include "../Input/Input.h"
#include "../Input/JoyPad.h"
#include "../Input/Keyboard.h"
#include "../Math/Math.h"

ResultScene::ResultScene(int score) :
    SceneBase(),
    mScore(score) {
}

ResultScene::~ResultScene() = default;

void ResultScene::start() {
    auto res = GameObjectCreater::createUI("Result");
    auto result = res->componentManager()->getComponent<Result>();
    result->setScore(mScore);
}

void ResultScene::update() {
    if (Input::keyboard()->getKeyDown(KeyCode::Space) || Input::joyPad()->getJoyDown(JoyCode::A)) {
        nextScene(std::make_shared<Title>());
    }
}

#include "Result.h"
#include "ComponentManager.h"
#include "Text.h"
#include "../GameObject/GameObject.h"
#include "../Utility/StringUtil.h"

Result::Result(std::shared_ptr<GameObject> owner) :
    Component(owner, "Result"),
    mText(nullptr),
    mScore(0) {
}

Result::~Result() = default;

void Result::start() {
    mText = owner()->componentManager()->getComponent<Text>();
}

void Result::update() {
    if (mText) {
        mText->setText("Score\n" + StringUtil::intToString(mScore));
    }
}

void Result::setScore(int score) {
    mScore = score;
}

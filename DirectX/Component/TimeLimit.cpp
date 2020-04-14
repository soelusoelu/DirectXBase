#include "TimeLimit.h"
#include "ComponentManager.h"
#include "Text.h"
#include "Timer.h"
#include "../Device/DrawString.h"
#include "../GameObject/GameObject.h"
#include "../System/Window.h"
#include "../Utility/StringUtil.h"

TimeLimit::TimeLimit(std::shared_ptr<GameObject> owner) :
    Component(owner, "TimeLimit"),
    mText(nullptr),
    mTimer(nullptr) {
}

TimeLimit::~TimeLimit() = default;

void TimeLimit::start() {
    mText = owner()->componentManager()->getComponent<Text>();
    mTimer = owner()->componentManager()->getComponent<Timer>();
}

void TimeLimit::update() {
    auto pos = 1920.f / 2.f;
    auto s = StringUtil::floatToString(mTimer->currentDownTime());
    pos -= s.length() * DrawString::WIDTH * mText->getScale().x / 2.f;
    if (mText) {
        mText->setText(s);
        mText->setPosition(Vector2(pos, 0.f));
    }
}

#include "TimeLimit.h"
#include "ComponentManager.h"
#include "Text.h"
#include "../Device/DrawString.h"
#include "../Device/Time.h"
#include "../GameObject/GameObject.h"
#include "../System/Window.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

TimeLimit::TimeLimit(std::shared_ptr<GameObject> owner) :
    Component(owner, "TimeLimit"),
    mTimer(std::make_unique<Time>(0.f)),
    mText(nullptr),
    mLimit(0.f) {
}

TimeLimit::~TimeLimit() = default;

void TimeLimit::start() {
    mText = owner()->componentManager()->getComponent<Text>();

    mTimer->setLimitTime(mLimit);
}

void TimeLimit::update() {
    mTimer->update();


    auto pos = 1920.f / 2.f;
    auto s = StringUtil::floatToString(mTimer->limitTime() - mTimer->currentTime());
    pos -= s.length() * DrawString::WIDTH * mText->getScale().x / 2.f;
    if (mText) {
        mText->setText(s);
        mText->setPosition(Vector2(pos, 0.f));
    }
}

void TimeLimit::loadProperties(const rapidjson::Value & inObj) {
    Component::loadProperties(inObj);

    JsonHelper::getFloat(inObj, "limit", &mLimit);
}

void TimeLimit::saveProperties(rapidjson::Document::AllocatorType & alloc, rapidjson::Value * inObj) const {
    Component::saveProperties(alloc, inObj);

    JsonHelper::setFloat(alloc, inObj, "limit", mLimit);
}

bool TimeLimit::isOverLimit() const {
    return mTimer->isTime();
}

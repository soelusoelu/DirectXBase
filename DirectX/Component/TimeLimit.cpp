#include "TimeLimit.h"
#include "../Device/DrawString.h"
#include "../Device/Renderer.h"
#include "../Device/Time.h"
#include "../GameObject/GameObject.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

TimeLimit::TimeLimit(std::shared_ptr<GameObject> owner) :
    Component(owner, "TimeLimit"),
    mTimer(std::make_unique<Time>(0.f)),
    mLimit(0.f),
    mDrawPosition(Vector2::zero),
    mDrawScale(Vector2::one) {
}

TimeLimit::~TimeLimit() = default;

void TimeLimit::start() {
    mTimer->setLimitTime(mLimit);
}

void TimeLimit::update() {
    mTimer->update();

    auto ds = owner()->renderer()->getDrawString();
    auto s = StringUtil::floatToString(mTimer->limitTime() - mTimer->currentTime());
    auto pos = mDrawPosition;
    pos.x -= s.length() * DrawString::WIDTH * mDrawScale.x / 2.f;
    ds->drawString(s, pos, mDrawScale);
}

void TimeLimit::loadProperties(const rapidjson::Value & inObj) {
    Component::loadProperties(inObj);

    JsonHelper::getFloat(inObj, "limit", &mLimit);
    JsonHelper::getVector2(inObj, "position", &mDrawPosition);
    JsonHelper::getVector2(inObj, "scale", &mDrawScale);
}

void TimeLimit::saveProperties(rapidjson::Document::AllocatorType & alloc, rapidjson::Value * inObj) const {
    Component::saveProperties(alloc, inObj);

    JsonHelper::setFloat(alloc, inObj, "limit", mLimit);
    JsonHelper::setVector2(alloc, inObj, "position", mDrawPosition);
    JsonHelper::setVector2(alloc, inObj, "scale", mDrawScale);
}

bool TimeLimit::isOverLimit() const {
    return mTimer->isTime();
}

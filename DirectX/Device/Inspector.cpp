#include "Inspector.h"
#include "DrawString.h"
#include "../Actor/Actor.h"
#include "../Actor/Transform3D.h"
#include "../Component/Component.h"
#include "../Component/ComponentManager.h"
#include "../System/Window.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"
#include <list>
#include <string>

Inspector::Inspector(const std::shared_ptr<DrawString> drawString) :
    mDrawString(drawString),
    mTagScale(Vector2::one),
    mTransformScale(Vector2::one),
    mTransformPosition(Vector2::zero),
    mTransformElementPositionX(0.f),
    mComponentPosition(Vector2::zero)
{
}

Inspector::~Inspector() = default;

void Inspector::loadProperties(const rapidjson::Value & inObj) {
    const auto& obj = inObj["inspector"];
    if (obj.IsObject()) {
        JsonHelper::getVector2(obj, "tagScale", &mTagScale);
        JsonHelper::getVector2(obj, "transformScale", &mTransformScale);
    }
}

void Inspector::initialize() {
    mTransformPosition = Vector2(Window::width(), DrawString::HEIGHT * mTagScale.y + DrawString::HEIGHT * mTransformScale.y);
    mTransformElementPositionX = Window::width() + DrawString::WIDTH * mTransformScale.x * 11;
    mComponentPosition = mTransformPosition;
    mComponentPosition.y += DrawString::HEIGHT * mTransformScale.y * 5;
}

void Inspector::setTarget(const ActorPtr target) {
    mTarget = target;
}

void Inspector::drawInspect() const {
    auto actor = mTarget.lock();
    if (!actor) {
        return;
    }

    drawActorTag(actor);
    drawActorTransform(actor->transform());

    auto compList = actor->componentManager()->getAllComponents();
    //アクターがコンポーネントを所持していなければ終了
    if (compList.empty()) {
        return;
    }

    auto drawPos = mComponentPosition;
    //全コンポーネントの情報を表示
    for (const auto& comp : compList) {
        drawComponent(comp, &drawPos);
        drawPos.x = Window::width();
        drawPos.y += DrawString::HEIGHT * mTransformScale.y;
    }
}

void Inspector::drawActorTag(const ActorPtr target) const {
    auto tag = target->tag();
    auto pos = TAG_POSITION;
    pos.x -= DrawString::WIDTH * mTagScale.x * tag.length() / 2.f;
    mDrawString->drawString(tag, pos, mTagScale);
}

void Inspector::drawActorTransform(const TransformPtr target) const {
    auto pos = mTransformPosition;
    mDrawString->drawString("Transform", pos, mTransformScale);
    pos.x += DrawString::WIDTH * mTransformScale.x * 2.f;
    const static float offsetY = DrawString::HEIGHT * mTransformScale.y;
    pos.y += offsetY;
    //Transformの各要素を描画
    drawPosition(target, pos);
    pos.y += offsetY;
    drawRotation(target, pos);
    pos.y += offsetY;
    drawScale(target, pos);
    pos.y += offsetY;
}

void Inspector::drawPosition(const TransformPtr target, const Vector2& position) const {
    auto pos = position;
    std::string str = "position";
    mDrawString->drawString(str, pos, mTransformScale);
    pos.x = mTransformElementPositionX;
    mDrawString->drawString("X " + StringUtil::floatToString(target->getPosition().x, 2), pos, mTransformScale);
    const static float offset = DrawString::WIDTH * mTransformScale.x * 8;
    pos.x += offset;
    mDrawString->drawString("Y " + StringUtil::floatToString(target->getPosition().y, 2), pos, mTransformScale);
    pos.x += offset;
    mDrawString->drawString("Z " + StringUtil::floatToString(target->getPosition().z, 2), pos, mTransformScale);
}

void Inspector::drawRotation(const TransformPtr target, const Vector2& position) const {
    auto pos = position;
    std::string str = "rotation";
    mDrawString->drawString(str, pos, mTransformScale);
    pos.x = mTransformElementPositionX;
    auto rot = target->getRotation();
    mDrawString->drawString("X " + StringUtil::floatToString(rot.x, 2), pos, mTransformScale);
    const static float offset = DrawString::WIDTH * mTransformScale.x * 8;
    pos.x += offset;
    mDrawString->drawString("Y " + StringUtil::floatToString(rot.y, 2), pos, mTransformScale);
    pos.x += offset;
    mDrawString->drawString("Z " + StringUtil::floatToString(rot.z, 2), pos, mTransformScale);
    pos.x += offset;
    mDrawString->drawString("W " + StringUtil::floatToString(rot.w, 2), pos, mTransformScale);
}

void Inspector::drawScale(const TransformPtr target, const Vector2& position) const {
    auto pos = position;
    std::string str = "scale";
    mDrawString->drawString(str, pos, mTransformScale);
    pos.x = mTransformElementPositionX;
    auto scale = target->getScale();
    mDrawString->drawString("X " + StringUtil::floatToString(scale.x, 2), pos, mTransformScale);
    const static float offset = DrawString::WIDTH * mTransformScale.x * 8;
    pos.x += offset;
    mDrawString->drawString("Y " + StringUtil::floatToString(scale.y, 2), pos, mTransformScale);
    pos.x += offset;
    mDrawString->drawString("Z " + StringUtil::floatToString(scale.z, 2), pos, mTransformScale);
}

void Inspector::drawComponent(const ComponentPtr component, Vector2* position) const {
    auto pos = *position;
    mDrawString->drawString(component->getTypeName(), pos, mTransformScale);

    pos.x = mTransformElementPositionX;
    pos.y += DrawString::HEIGHT * mTransformScale.y;

    *position = pos;
}

const Vector2 Inspector::TAG_POSITION = Vector2(Window::width() + (Window::debugWidth() - Window::width()) / 2.f, 0.f);

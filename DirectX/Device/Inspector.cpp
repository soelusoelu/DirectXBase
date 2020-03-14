#include "Inspector.h"
#include "DrawString.h"
#include "../Actor/Actor.h"
#include "../Actor/Transform3D.h"
#include "../Component/Component.h"
#include "../Component/ComponentManager.h"
#include "../System/Window.h"
#include "../Utility/StringUtil.h"
#include <list>
#include <string>

Inspector::Inspector(const std::shared_ptr<DrawString> drawString) :
    mDrawString(drawString) {
}

Inspector::~Inspector() = default;

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

    auto drawPos = Vector2(Window::width(), 64.f * 3);
    //全コンポーネントの情報を表示
    for (const auto& comp : compList) {
        mDrawString->drawString(comp->getTypeName(), drawPos, Vector2(0.3f, 0.3f));
        drawPos.y += 64.f;
    }
}

void Inspector::drawActorTag(const ActorPtr target) const {
    auto tag = target->tag();
    auto pos = TAG_POSITION;
    pos.x -= DrawString::WIDTH * TAG_SCALE.x * tag.length() / 2.f;
    mDrawString->drawString(tag, pos, TAG_SCALE);
}

void Inspector::drawActorTransform(const TransformPtr target) const {
    auto pos = TRANSFORM_POSITION;
    mDrawString->drawString("Transform", pos, TRANSFORM_SCALE);
    pos.x += DrawString::WIDTH * TRANSFORM_SCALE.x * 2.f;
    const static float offsetY = DrawString::HEIGHT * TRANSFORM_SCALE.y;
    pos.y += offsetY;
    //Transformの各要素を描画
    drawPosition(target, pos);
    pos.y += offsetY;
    drawRotation(target, pos);
    pos.y += offsetY;
    drawScale(target, pos);
    pos.y += offsetY;
}

void Inspector::drawPosition(const TransformPtr target, const Vector2& pos) const {
    auto p = pos;
    std::string str = "position";
    mDrawString->drawString(str, p, TRANSFORM_SCALE);
    p.x = TRANSFORM_ELEMENT_POSITION_X;
    mDrawString->drawString("X " + StringUtil::floatToString(target->getPosition().x, 2), p, TRANSFORM_SCALE);
    const static float offset = DrawString::WIDTH * TRANSFORM_SCALE.x * 8;
    p.x += offset;
    mDrawString->drawString("Y " + StringUtil::floatToString(target->getPosition().y, 2), p, TRANSFORM_SCALE);
    p.x += offset;
    mDrawString->drawString("Z " + StringUtil::floatToString(target->getPosition().z, 2), p, TRANSFORM_SCALE);
}

void Inspector::drawRotation(const TransformPtr target, const Vector2& pos) const {
    auto p = pos;
    std::string str = "rotation";
    mDrawString->drawString(str, p, TRANSFORM_SCALE);
    p.x = TRANSFORM_ELEMENT_POSITION_X;
    auto rot = target->getRotation();
    mDrawString->drawString("X " + StringUtil::floatToString(rot.x, 2), p, TRANSFORM_SCALE);
    const static float offset = DrawString::WIDTH * TRANSFORM_SCALE.x * 8;
    p.x += offset;
    mDrawString->drawString("Y " + StringUtil::floatToString(rot.y, 2), p, TRANSFORM_SCALE);
    p.x += offset;
    mDrawString->drawString("Z " + StringUtil::floatToString(rot.z, 2), p, TRANSFORM_SCALE);
    p.x += offset;
    mDrawString->drawString("W " + StringUtil::floatToString(rot.w, 2), p, TRANSFORM_SCALE);
}

void Inspector::drawScale(const TransformPtr target, const Vector2& pos) const {
    auto p = pos;
    std::string str = "scale";
    mDrawString->drawString(str, p, TRANSFORM_SCALE);
    p.x = TRANSFORM_ELEMENT_POSITION_X;
    auto scale = target->getScale();
    mDrawString->drawString("X " + StringUtil::floatToString(scale.x, 2), p, TRANSFORM_SCALE);
    const static float offset = DrawString::WIDTH * TRANSFORM_SCALE.x * 8;
    p.x += offset;
    mDrawString->drawString("Y " + StringUtil::floatToString(scale.y, 2), p, TRANSFORM_SCALE);
    p.x += offset;
    mDrawString->drawString("Z " + StringUtil::floatToString(scale.z, 2), p, TRANSFORM_SCALE);
}

const Vector2 Inspector::TAG_POSITION = Vector2(Window::width() + (Window::debugWidth() - Window::width()) / 2.f, 0.f);
const Vector2 Inspector::TAG_SCALE = Vector2(0.5f, 0.5f);
const Vector2 Inspector::TRANSFORM_POSITION = Vector2(Window::width(), DrawString::HEIGHT * TAG_SCALE.y * 2);
const Vector2 Inspector::TRANSFORM_SCALE = Vector2(0.3f, 0.3f);
const float Inspector::TRANSFORM_ELEMENT_POSITION_X = Window::width() + DrawString::WIDTH * TRANSFORM_SCALE.x * 11;

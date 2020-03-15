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
    mElementScale(Vector2::one),
    mTransformPosition(Vector2::zero),
    mComponentPosition(Vector2::zero),
    mElementPositionX(0.f),
    mValuePositionX(0.f),
    mCharWidth(0.f),
    mCharHeight(0.f) {
}

Inspector::~Inspector() = default;

void Inspector::loadProperties(const rapidjson::Value & inObj) {
    const auto& obj = inObj["inspector"];
    if (obj.IsObject()) {
        JsonHelper::getVector2(obj, "tagScale", &mTagScale);
        JsonHelper::getVector2(obj, "transformScale", &mElementScale);
    }
}

void Inspector::initialize() {
    //1文字のサイズを基準に描画位置を決める
    mCharWidth = DrawString::WIDTH * mElementScale.x;
    mCharHeight = DrawString::HEIGHT * mElementScale.y;

    mTransformPosition = Vector2(Window::width(), DrawString::HEIGHT * mTagScale.y + mCharHeight);
    mComponentPosition = mTransformPosition;
    mComponentPosition.y += mCharHeight * 5;
    mElementPositionX = Window::width() + mCharWidth * 2;
    mValuePositionX = Window::width() + mCharWidth * 11;
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
        drawPos.y += mCharHeight * 2;
    }
}

void Inspector::drawActorTag(const ActorPtr target) const {
    auto tag = target->tag();
    auto pos = Vector2(Window::width() + (Window::debugWidth() - Window::width()) / 2.f, 0.f);
    pos.x -= DrawString::WIDTH * mTagScale.x * tag.length() / 2.f;
    mDrawString->drawString(tag, pos, mTagScale);
}

void Inspector::drawActorTransform(const TransformPtr target) const {
    auto pos = mTransformPosition;
    mDrawString->drawString("Transform", pos, mElementScale);
    pos.x = mElementPositionX;
    pos.y += mCharHeight;
    //Transformの各要素を描画
    drawPosition(target, pos);
    pos.y += mCharHeight;
    drawRotation(target, pos);
    pos.y += mCharHeight;
    drawScale(target, pos);
}

void Inspector::drawPosition(const TransformPtr target, const Vector2 & position) const {
    auto pos = position;
    mDrawString->drawString("Position", pos, mElementScale);
    pos.x = mValuePositionX;
    auto tPos = target->getPosition();
    mDrawString->drawString(InspectHelper::vector3ToString(tPos), pos, mElementScale);
}

void Inspector::drawRotation(const TransformPtr target, const Vector2 & position) const {
    auto pos = position;
    mDrawString->drawString("Rotation", pos, mElementScale);
    pos.x = mValuePositionX;
    auto rot = target->getRotation();
    mDrawString->drawString(InspectHelper::quaternionToString(rot), pos, mElementScale);
}

void Inspector::drawScale(const TransformPtr target, const Vector2 & position) const {
    auto pos = position;
    mDrawString->drawString("Scale", pos, mElementScale);
    pos.x = mValuePositionX;
    auto scale = target->getScale();
    mDrawString->drawString(InspectHelper::vector3ToString(scale), pos, mElementScale);
}

void Inspector::drawComponent(const ComponentPtr component, Vector2 * position) const {
    auto pos = *position;
    mDrawString->drawString(component->getTypeName(), pos, mElementScale);

    //コンポーネントのデバッグ情報を取得
    std::list<std::pair<std::string, std::string>> debugInfo;
    component->drawDebugInfo(&debugInfo);

    //すべてのデバッグ情報を描画
    for (const auto& info : debugInfo) {
        pos.x = mElementPositionX;
        pos.y += mCharHeight;
        auto sub = info.first.substr(0, 8);
        mDrawString->drawString(sub, pos, mElementScale);
        pos.x = mValuePositionX;
        mDrawString->drawString(info.second, pos, mElementScale);
    }

    *position = pos;
}

std::string InspectHelper::vector2ToString(const Vector2 & vec) {
    return "X " + StringUtil::floatToString(vec.x, 2) + "  Y " + StringUtil::floatToString(vec.y, 2);
}

std::string InspectHelper::vector3ToString(const Vector3 & vec) {
    return "X " + StringUtil::floatToString(vec.x, 2) + "  Y " + StringUtil::floatToString(vec.y, 2) + "  Z " + StringUtil::floatToString(vec.z, 2);
}

std::string InspectHelper::quaternionToString(const Quaternion & quaternion) {
    return "X " + StringUtil::floatToString(quaternion.x, 2) +
        "  Y " + StringUtil::floatToString(quaternion.y, 2) +
        "  Z " + StringUtil::floatToString(quaternion.z, 2) +
        "  W " + StringUtil::floatToString(quaternion.w, 2);
}

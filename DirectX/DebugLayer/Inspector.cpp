#include "Inspector.h"
#include "../Component/Component.h"
#include "../Component/ComponentManager.h"
#include "../Device/DrawString.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"
#include "../System/Window.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"
#include <list>
#include <string>

Inspector::Inspector(DrawString* drawString) :
    mDrawString(drawString),
    mInspectorPositionX(0.f),
    mTagScale(Vector2::one),
    mElementScale(Vector2::one),
    mTransformPosition(Vector2::zero),
    mComponentPosition(Vector2::zero),
    mElementPositionX(0.f),
    mValuePositionX(0.f),
    mOffsetCharCountX(0),
    mOffsetX(0.f),
    mCharWidth(0.f),
    mCharHeight(0.f),
    mMaxElementCharCount(0) {
}

Inspector::~Inspector() = default;

void Inspector::loadProperties(const rapidjson::Value & inObj) {
    const auto& obj = inObj["inspector"];
    if (obj.IsObject()) {
        JsonHelper::getFloat(obj, "inspectorPositionX", &mInspectorPositionX);
        JsonHelper::getVector2(obj, "tagScale", &mTagScale);
        JsonHelper::getVector2(obj, "elementScale", &mElementScale);
        JsonHelper::getInt(obj, "offsetCharCountX", &mOffsetCharCountX);
    }
}

void Inspector::initialize() {
    //1文字のサイズを基準に描画位置を決める
    mCharWidth = DrawString::WIDTH * mElementScale.x;
    mCharHeight = DrawString::HEIGHT * mElementScale.y;

    mOffsetX = mCharWidth * mOffsetCharCountX;
    mTransformPosition = Vector2(mInspectorPositionX + mOffsetX, DrawString::HEIGHT * mTagScale.y + mCharHeight);
    mComponentPosition = mTransformPosition;
    mComponentPosition.y += mCharHeight * 5;
    mElementPositionX = mTransformPosition.x + mCharWidth * 2;
    mValuePositionX = mTransformPosition.x + mCharWidth * 14;
    mMaxElementCharCount = (mValuePositionX - mElementPositionX) / mCharWidth - 1;
}

void Inspector::setTarget(const GameObjectPtr target) {
    mTarget = target;
}

void Inspector::drawInspect() const {
    auto actor = mTarget.lock();
    if (!actor) {
        return;
    }

    drawTag(actor);
    drawTransform(actor->transform());

    auto compList = actor->componentManager()->getAllComponents();
    //アクターがコンポーネントを所持していなければ終了
    if (compList.empty()) {
        return;
    }

    auto drawPos = mComponentPosition;
    //全コンポーネントの情報を表示
    for (const auto& comp : compList) {
        drawComponent(comp, &drawPos);
        drawPos.x = mComponentPosition.x;
        drawPos.y += mCharHeight * 2;
    }
}

void Inspector::drawTag(const GameObjectPtr target) const {
    auto tag = target->tag();
    auto pos = Vector2(mInspectorPositionX + (Window::debugWidth() - mInspectorPositionX) / 2.f, 0.f);
    pos.x -= DrawString::WIDTH * mTagScale.x * tag.length() / 2.f;
    mDrawString->drawString(tag, pos, mTagScale);
}

void Inspector::drawTransform(const TransformPtr target) const {
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
        auto ele = info.first;
        if (ele.length() > mMaxElementCharCount) {
            ele = ele.substr(0, mMaxElementCharCount);
        }
        pos.y += mCharHeight;
        mDrawString->drawString(ele, pos, mElementScale);
        pos.x = mValuePositionX;
        mDrawString->drawString(info.second, pos, mElementScale);
    }

    *position = pos;
}

std::string InspectHelper::vector2ToString(const Vector2 & vec) {
    auto x = "X " + minusPosition(vec.x);
    x.resize(10);
    auto y = "Y " + minusPosition(vec.y);
    return x + y;
}

std::string InspectHelper::vector3ToString(const Vector3 & vec) {
    auto x = "X " + minusPosition(vec.x);
    x.resize(10);
    auto y = "Y " + minusPosition(vec.y);
    y.resize(10);
    auto z = "Z " + minusPosition(vec.z);
    return x + y + z;
}

std::string InspectHelper::quaternionToString(const Quaternion & quaternion) {
    auto x = "X " + minusPosition(quaternion.x);
    x.resize(10);
    auto y = "Y " + minusPosition(quaternion.y);
    y.resize(10);
    auto z = "Z " + minusPosition(quaternion.z);
    z.resize(10);
    auto w = "W " + minusPosition(quaternion.w);
    return x + y + z + w;
}

std::string InspectHelper::minusPosition(float value) {
    auto s = StringUtil::floatToString(value);
    return (value < 0.f) ? s : " " + s;
}

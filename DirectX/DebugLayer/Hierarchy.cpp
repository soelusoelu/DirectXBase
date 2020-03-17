#include "Hierarchy.h"
#include "../Actor/Actor.h"
#include "../Device/DrawString.h"
#include "../System/Window.h"
#include "../Utility/LevelLoader.h"

Hierarchy::Hierarchy(DrawString* drawString) :
    mDrawString(drawString),
    mPosition(Vector2::zero),
    mScale(Vector2::one),
    mOffsetCharCountX(0),
    mOffsetCharCountY(0),
    mCharWidth(0.f),
    mCharHeight(0.f) {
}

Hierarchy::~Hierarchy() = default;

void Hierarchy::loadProperties(const rapidjson::Value & inObj) {
    const auto& obj = inObj["hierarchy"];
    if (obj.IsObject()) {
        JsonHelper::getVector2(obj, "scale", &mScale);
        JsonHelper::getInt(obj, "offsetCharCountX", &mOffsetCharCountX);
        JsonHelper::getInt(obj, "offsetCharCountY", &mOffsetCharCountY);
    }
}

void Hierarchy::initialize() {
    mCharWidth = DrawString::WIDTH * mScale.x;
    mCharHeight = DrawString::HEIGHT * mScale.y;

    mPosition = Vector2(Window::width(), 0.f);
    mPosition += Vector2(mOffsetCharCountX * mCharWidth, mOffsetCharCountY * mCharHeight);
}

void Hierarchy::drawActors(const std::list<std::shared_ptr<Actor>> actors) {
    auto drawPos = mPosition;
    for (const auto& actor : actors) {
        mDrawString->drawString(actor->tag(), drawPos, mScale);
        drawPos.y += DrawString::HEIGHT * mScale.y;
    }
}

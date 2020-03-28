#include "JumpTarget.h"
#include "../Actor/Transform2D.h"
#include "../Sprite/Sprite.h"
#include "../Utility/LevelLoader.h"

JumpTarget::JumpTarget(std::shared_ptr<Renderer> renderer) :
    UI(renderer, "JumpTarget"),
    mPosition(Vector2::zero),
    mTextureScale(Vector2::one) {
}

JumpTarget::~JumpTarget() = default;

void JumpTarget::start() {
    auto s = std::make_shared<Sprite>(mRenderer, "circleP.png");
    s->transform()->setScale(mTextureScale);
    addSprite(s);
}

void JumpTarget::updateUI() {
}

void JumpTarget::loadProperties(const rapidjson::Value & inObj) {
    UI::loadProperties(inObj);

    JsonHelper::getVector2(inObj, "textureScale", &mTextureScale);
}

void JumpTarget::saveProperties(rapidjson::Document::AllocatorType & alloc, rapidjson::Value * inObj) const {
    UI::saveProperties(alloc, inObj);

    JsonHelper::setVector2(alloc, inObj, "textureScale", mTextureScale);
}

void JumpTarget::setTargetPosition(const Vector2 & pos) {
    mPosition = pos;
}

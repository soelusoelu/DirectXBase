#include "ResultChickenFall.h"
#include "ComponentManager.h"
#include "SpriteComponent.h"
#include "../Device/Random.h"
#include "../Device/Time.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform2D.h"
#include "../System/Window.h"
#include "../Utility/LevelLoader.h"

ResultChickenFall::ResultChickenFall() :
    Component(),
    mSprite(nullptr),
    mRandomFallSpeedMin(0.f),
    mRandomFallSpeedMax(0.f),
    mFallSpeed(0.f),
    mRotateSpeed(0.f) {
}

ResultChickenFall::~ResultChickenFall() = default;

void ResultChickenFall::start() {
    mSprite = owner()->componentManager()->getComponent<SpriteComponent>();
    const auto& t = mSprite->transform();
    t->setPivot(Pivot::CENTER);
    initialize();
    auto randomPosY = Random::randomRange(0.f, static_cast<float>(Window::standardHeight()));
    t->translate(Vector2::up * randomPosY);
}

void ResultChickenFall::update() {
    const auto& t = mSprite->transform();
    t->translate(Vector2::down * mFallSpeed * Time::deltaTime);
    t->rotate(mRotateSpeed * Time::deltaTime);

    if (t->getPosition().y > Window::standardHeight() + mSprite->getTextureSize().y) {
        initialize();
    }
}

void ResultChickenFall::loadProperties(const rapidjson::Value& inObj) {
    JsonHelper::getFloat(inObj, "randomFallSpeedMin", &mRandomFallSpeedMin);
    JsonHelper::getFloat(inObj, "randomFallSpeedMax", &mRandomFallSpeedMax);
    JsonHelper::getFloat(inObj, "rotateSpeed", &mRotateSpeed);
}

void ResultChickenFall::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("FallSpeed", mFallSpeed);
}

void ResultChickenFall::initialize() {
    auto randomPosX = Random::randomRange(0.f, static_cast<float>(Window::standardWidth()));
    mSprite->transform()->setPosition(Vector2(randomPosX, -mSprite->getTextureSize().y));

    mFallSpeed = Random::randomRange(mRandomFallSpeedMin, mRandomFallSpeedMax);
}

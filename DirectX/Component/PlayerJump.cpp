#include "PlayerJump.h"
#include "../Device/Subject.h"
#include "../Device/Time.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"
#include "../Utility/LevelLoader.h"
#include <string>

PlayerJump::PlayerJump() :
    Component(),
    mJumpStartSubject(std::make_unique<Subject>()),
    mJumpEndSubject(std::make_unique<Subject>()),
    mCurrentState(State::NONE),
    mJumpStartPosition(Vector3::zero),
    mJumpTargetPosition(Vector3::zero),
    mJumpKey(KeyCode::None),
    mJumpPad(JoyCode::None),
    mSpeed(0.f),
    mMoveRate(0.f),
    mCanJump(false) {
}

PlayerJump::~PlayerJump() = default;

void PlayerJump::loadProperties(const rapidjson::Value & inObj) {
    std::string src;
    if (JsonHelper::getString(inObj, "jumpKey", &src)) {
        Keyboard::stringToKeyCode(src, &mJumpKey);
    }
    if (JsonHelper::getString(inObj, "jumpPad", &src)) {
        JoyPad::stringToJoyCode(src, &mJumpPad);
    }

    JsonHelper::getFloat(inObj, "speed", &mSpeed);
}

void PlayerJump::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("Speed", mSpeed);
    inspect->emplace_back("MoveRate", mMoveRate);
    inspect->emplace_back("CanJump", mCanJump);

    ComponentDebug::DebugInfo info;
    info.first = "State";
    if (mCurrentState == State::JUMPING) {
        info.second = "JUMPING";
    } else if (mCurrentState == State::NONE) {
        info.second = "NONE";
    }
    inspect->emplace_back(info);
}

bool PlayerJump::isJumping() const {
    return (mCurrentState == State::JUMPING);
}

void PlayerJump::setTargetPosition(const Vector3 & pos) {
    mJumpTargetPosition = pos;
}

void PlayerJump::canJump(bool value) {
    mCanJump = value;
}

void PlayerJump::onJumpStart(const std::function<void()>& f) {
    mJumpStartSubject->addObserver(f);
}

void PlayerJump::onJumpEnd(const std::function<void()>& f) {
    mJumpEndSubject->addObserver(f);
}

void PlayerJump::jump() {
    if (!mCanJump) {
        return;
    }

    bool isPressed = Input::joyPad()->getJoyDown(mJumpPad);
#ifdef _DEBUG
    if (!isPressed) {
        isPressed = Input::keyboard()->getKeyDown(mJumpKey);
    }
#endif // _DEBUG
    if (!isPressed) {
        return;
    }

    startInitialize();
}

void PlayerJump::jumpUpdate() {
    move();
    stop();
}

void PlayerJump::move() {
    mMoveRate += Time::deltaTime / mSpeed;
    auto l = Vector3::lerp(mJumpStartPosition, mJumpTargetPosition, mMoveRate);
    owner()->transform()->setPosition(l);
}

void PlayerJump::stop() {
    if (mMoveRate >= 1.f) {
        mJumpEndSubject->notify();
        mCurrentState = State::NONE;
    }
}

void PlayerJump::startInitialize() {
    mCurrentState = State::JUMPING;
    mJumpStartPosition = owner()->transform()->getPosition();
    mMoveRate = 0.f;
    mJumpStartSubject->notify();
}

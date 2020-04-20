#include "PlayerJump.h"
#include "../Device/Time.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"
#include "../Input/Input.h"
#include "../Input/JoyPad.h"
#include "../Input/Keyboard.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"
#include <string>

PlayerJump::PlayerJump(std::shared_ptr<GameObject> owner) :
    Component(owner, "PlayerJump", 5),
    mCurrentState(State::NONE),
    mPreviousState(State::NONE),
    mJumpStartPosition(Vector3::zero),
    mJumpTargetPosition(Vector3::zero),
    mJumpKey(KeyCode::None),
    mJumpPad(JoyCode::None),
    mSpeed(0.f),
    mMoveRate(0.f) {
}

PlayerJump::~PlayerJump() = default;

void PlayerJump::update() {
    mPreviousState = mCurrentState;
}

void PlayerJump::loadProperties(const rapidjson::Value & inObj) {
    Component::loadProperties(inObj);

    std::string src;
    if (JsonHelper::getString(inObj, "jumpKey", &src)) {
        Keyboard::stringToKeyCode(src, &mJumpKey);
    }
    if (JsonHelper::getString(inObj, "jumpPad", &src)) {
        JoyPad::stringToJoyCode(src, &mJumpPad);
    }

    JsonHelper::getFloat(inObj, "speed", &mSpeed);
}

void PlayerJump::drawDebugInfo(DebugInfoList * inspect) const {
    Component::drawDebugInfo(inspect);

    DebugInfo info;
    info.first = "Speed";
    info.second = StringUtil::floatToString(mSpeed);
    inspect->emplace_back(info);
    info.first = "MoveRate";
    info.second = StringUtil::floatToString(mMoveRate);
    inspect->emplace_back(info);

    info.first = "State";
    if (mCurrentState == State::JUMPING) {
        info.second = "JUMPING";
    } else if (mCurrentState == State::NONE) {
        info.second = "NONE";
    }
    inspect->emplace_back(info);
}

void PlayerJump::jump() {
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

bool PlayerJump::isJumpStart() const {
    return (mCurrentState == State::JUMPING && mPreviousState == State::NONE);
}

bool PlayerJump::isJumping() const {
    return (mCurrentState == State::JUMPING);
}

bool PlayerJump::isJumpEnd() const {
    return (mCurrentState == State::NONE && mPreviousState == State::JUMPING);
}

void PlayerJump::setTargetPosition(const Vector3 & pos) {
    mJumpTargetPosition = pos;
}

void PlayerJump::move() {
    mMoveRate += Time::deltaTime * mSpeed;
    auto l = Vector3::lerp(mJumpStartPosition, mJumpTargetPosition, mMoveRate);
    owner()->transform()->setPosition(l);
}

void PlayerJump::stop() {
    if (mMoveRate >= 1.f) {
        mCurrentState = State::NONE;
    }
}

void PlayerJump::startInitialize() {
    mCurrentState = State::JUMPING;
    mJumpStartPosition = owner()->transform()->getPosition();
    mMoveRate = 0.f;
}

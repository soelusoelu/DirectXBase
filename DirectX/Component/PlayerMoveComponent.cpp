#include "PlayerMoveComponent.h"
#include "../Component/ComponentManager.h"
#include "../Component/MeshComponent.h"
#include "../Device/Time.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"
#include "../Input/Input.h"
#include "../Input/JoyPad.h"
#include "../Input/Keyboard.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

PlayerMoveComponent::PlayerMoveComponent(std::shared_ptr<GameObject> owner) :
    Component(owner, "PlayerMoveComponent", 10),
    mState(State::WALK),
    mJumpStartPosition(Vector3::zero),
    mJumpTargetPosition(Vector3::zero),
    mMoveSpeed(1.f),
    mJumpMoveRate(0.f),
    mJumpStart(false),
    mJumpEnd(false),
    mIsWaitFirstFrame(false) {
}

PlayerMoveComponent::~PlayerMoveComponent() = default;

void PlayerMoveComponent::start() {
    auto mesh = owner()->componentManager()->getComponent<MeshComponent>();
    if (mesh) {
        owner()->transform()->setPivot(Vector3::down * mesh->getRadius());
    }
}

void PlayerMoveComponent::update() {
    if (!mIsWaitFirstFrame) {
        mIsWaitFirstFrame = true;
        return;
    }

    if (mJumpStart) {
        mJumpStart = false;
    }
    if (mJumpEnd) {
        mJumpEnd = false;
    }
    if (isWalking()) {
        jump();
        walk();
    } else if (isJumping()) {
        jumpMove();
        jumpStop();
    }
}

void PlayerMoveComponent::loadProperties(const rapidjson::Value & inObj) {
    Component::loadProperties(inObj);

    JsonHelper::getFloat(inObj, "moveSpeed", &mMoveSpeed);
}

void PlayerMoveComponent::drawDebugInfo(DebugInfoList * inspect) const {
    DebugInfo info;
    info.first = "State";
    if (mState == State::WALK) {
        info.second = "WALK";
    } else if (mState == State::JUMP) {
        info.second = "JUMP";
    }
    inspect->emplace_back(info);
    info.first = "MoveSpeed";
    info.second = StringUtil::floatToString(mMoveSpeed);
    inspect->emplace_back(info);
}

bool PlayerMoveComponent::isWalking() const {
    return mState == State::WALK;
}

bool PlayerMoveComponent::isJumpStart() const {
    return mJumpStart;
}

bool PlayerMoveComponent::isJumping() const {
    return mState == State::JUMP;
}

bool PlayerMoveComponent::isJumpEnd() const {
    return mJumpEnd;
}

void PlayerMoveComponent::setTargetPosition(const Vector3 & pos) {
    if (mState == State::JUMP) {
        return;
    }
    mJumpTargetPosition = pos;
}

void PlayerMoveComponent::jump() {
    if (Input::keyboard()->getKeyDown(KeyCode::Space) || Input::joyPad()->getJoyDown(JoyCode::A)) {
        jumpStartInitialize();
        mState = State::JUMP;
    }
}

void PlayerMoveComponent::walk() {
    if (Input::keyboard()->getKey(KeyCode::LeftShift)) {
        return;
    }

    auto left = Input::joyPad()->leftStick();
    if (!Math::nearZero(left.x) || !Math::nearZero(left.y)) {
        owner()->transform()->translate(Vector3(left.x, 0.f, left.y) * Time::deltaTime * mMoveSpeed);
        return;
    }

    auto h = Input::keyboard()->horizontal();
    auto v = Input::keyboard()->vertical();
    if (!Math::nearZero(h) || !Math::nearZero(v)) {
        owner()->transform()->translate(Vector3(h, 0.f, v) * Time::deltaTime * mMoveSpeed);
    }
}

void PlayerMoveComponent::jumpMove() {
    mJumpMoveRate += Time::deltaTime;
    auto l = Vector3::lerp(mJumpStartPosition, mJumpTargetPosition, mJumpMoveRate);
    auto t = owner()->transform();
    t->setPosition(l);
}

void PlayerMoveComponent::jumpStop() {
    if (mJumpMoveRate >= 1.f) {
        mJumpEnd = true;
        mState = State::WALK;
    }
}

void PlayerMoveComponent::jumpStartInitialize() {
    mJumpStart = true;
    mJumpStartPosition = owner()->transform()->getPosition();
    mJumpMoveRate = 0.f;
}

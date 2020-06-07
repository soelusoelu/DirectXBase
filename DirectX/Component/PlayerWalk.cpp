#include "PlayerWalk.h"
#include "Camera.h"
#include "ComponentManager.h"
#include "MeshComponent.h"
#include "../Device/Time.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectManager.h"
#include "../GameObject/Transform3D.h"
#include "../Utility/LevelLoader.h"
#include <string>

PlayerWalk::PlayerWalk() :
    Component(),
    mCamera(nullptr),
    mMoveDir(Vector3::zero),
    mRollKey(KeyCode::None),
    mRollPad(JoyCode::None),
    mMoveSpeed(1.f),
    mMeshRadius(0.f),
    mClampFore(0.f),
    mClampBack(0.f),
    mClampX(0.f) {
}

PlayerWalk::~PlayerWalk() = default;

void PlayerWalk::start() {
    const auto& camera = owner()->getGameObjectManager()->find("Camera");
    mCamera = camera->componentManager()->getComponent<Camera>();
    auto mesh = owner()->componentManager()->getComponent<MeshComponent>();
    if (mesh) {
        mMeshRadius = mesh->getRadius() * owner()->transform()->getScale().x;
    }
}

void PlayerWalk::loadProperties(const rapidjson::Value & inObj) {
    JsonHelper::getFloat(inObj, "moveSpeed", &mMoveSpeed);
    JsonHelper::getFloat(inObj, "clampFore", &mClampFore);
    JsonHelper::getFloat(inObj, "clampBack", &mClampBack);
    JsonHelper::getFloat(inObj, "clampX", &mClampX);
    std::string src;
    if (JsonHelper::getString(inObj, "rollKey", &src)) {
        Keyboard::stringToKeyCode(src, &mRollKey);
    }
    if (JsonHelper::getString(inObj, "rollPad", &src)) {
        JoyPad::stringToJoyCode(src, &mRollPad);
    }
}

void PlayerWalk::drawDebugInfo(ComponentDebug::DebugInfoList * inspect) const {
    inspect->emplace_back("MoveSpeed", mMoveSpeed);
    inspect->emplace_back("ClampFore", mClampFore);
    inspect->emplace_back("ClampBack", mClampBack);
    inspect->emplace_back("ClampX", mClampX);
}

const Vector3& PlayerWalk::getMoveDirection() const {
    return mMoveDir;
}

void PlayerWalk::walkUpdate() {
    mMoveDir = Vector3::zero;

    auto left = Input::joyPad()->leftStick();
    if (!Math::nearZero(left.x)) {
        mMoveDir.x = (left.x > 0.f) ? 1.f : -1.f;
    }
    if (!Math::nearZero(left.y)) {
        mMoveDir.z = (left.y > 0.f) ? 1.f : -1.f;
    }

    if (Input::joyPad()->getJoy(mRollPad)) {
        return;
    }

#ifdef _DEBUG
    auto h = Input::keyboard()->horizontal();
    auto v = Input::keyboard()->vertical();
    if (!Math::nearZero(h)) {
        mMoveDir.x = h;
    }
    if (!Math::nearZero(v)) {
        mMoveDir.z = v;
    }

    if (Input::keyboard()->getKey(mRollKey)) {
        return;
    }
#endif // _DEBUG

    if (!mCamera) {
        return;
    }

    auto amount = mMoveDir * Time::deltaTime * mMoveSpeed;
    auto pos = owner()->transform()->getPosition();
    auto clamp = pos;
    if (pos.z < mClampFore) {
        clamp.z = mClampFore;
    } else if (pos.z > mClampBack) {
        clamp.z = mClampBack;
    }
    if (pos.x > mClampX) {
        clamp.x = mClampX;
    } else if (pos.x < -mClampX) {
        clamp.x = -mClampX;
    }
    owner()->transform()->setPosition(clamp);
    
    pos = clamp;
    if (!mCamera->viewFrustumCulling(pos + amount, mMeshRadius)) {
        return;
    }

    owner()->transform()->translate(amount);
}

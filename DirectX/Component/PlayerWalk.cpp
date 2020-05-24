#include "PlayerWalk.h"
#include "Camera.h"
#include "ComponentManager.h"
#include "MeshComponent.h"
#include "../Device/Time.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectManager.h"
#include "../GameObject/Transform3D.h"
#include "../Input/Input.h"
#include "../Input/JoyPad.h"
#include "../Input/Keyboard.h"
#include "../Utility/LevelLoader.h"
#include <string>

PlayerWalk::PlayerWalk() :
    Component(),
    mCamera(nullptr),
    mMoveDir(Vector3::zero),
    mRollKey(KeyCode::None),
    mRollPad(JoyCode::None),
    mMoveSpeed(1.f),
    mMeshRadius(0.f) {
}

PlayerWalk::~PlayerWalk() = default;

void PlayerWalk::start() {
    auto camera = owner()->getGameObjectManager()->find("Camera");
    mCamera = camera->componentManager()->getComponent<Camera>();
    auto mesh = owner()->componentManager()->getComponent<MeshComponent>();
    if (mesh) {
        mMeshRadius = mesh->getRadius() * owner()->transform()->getScale().x;
    }
}

void PlayerWalk::loadProperties(const rapidjson::Value & inObj) {
    Component::loadProperties(inObj);

    JsonHelper::getFloat(inObj, "moveSpeed", &mMoveSpeed);
    std::string src;
    if (JsonHelper::getString(inObj, "rollKey", &src)) {
        Keyboard::stringToKeyCode(src, &mRollKey);
    }
    if (JsonHelper::getString(inObj, "rollPad", &src)) {
        JoyPad::stringToJoyCode(src, &mRollPad);
    }
}

void PlayerWalk::drawDebugInfo(DebugInfoList * inspect) const {
    Component::drawDebugInfo(inspect);

    DebugInfo info;
    info.first = "MoveSpeed";
    info.second = mMoveSpeed;
    inspect->emplace_back(info);
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
    if (!mCamera->viewFrustumCulling(pos + amount, mMeshRadius)) {
        return;
    }

    owner()->transform()->translate(amount);
}

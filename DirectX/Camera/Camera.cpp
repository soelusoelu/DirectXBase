#include "Camera.h"
#include "../Actor/PlayerActor.h"
#include "../Actor/Transform3D.h"
#include "../System/Window.h"
#include "../Utility/LevelLoader.h"

Camera::Camera() :
    mPosition(Vector3::zero),
    mLookAt(Vector3::zero),
    mUp(Vector3::up),
    mFOV(0.f),
    mNearClip(0.f),
    mFarClip(0.f),
    mView(Matrix4::identity),
    mProjection(Matrix4::identity) {
}

Camera::~Camera() = default;

void Camera::loadProperties(const rapidjson::Value & inObj) {
    const auto& obj = inObj["camera"];
    if (obj.IsObject()) {
        JsonHelper::getFloat(obj, "fov", &mFOV);
        JsonHelper::getFloat(obj, "nearClip", &mNearClip);
        JsonHelper::getFloat(obj, "farClip", &mFarClip);
    }
}

void Camera::initialize() {
    mPosition.set(5.f, 7.f, -14.f);
    mView = Matrix4::createLookAt(mPosition, mLookAt, mUp);
    mProjection = Matrix4::createPerspectiveFOV(mFOV, Window::width(), Window::height(), mNearClip, mFarClip);
}

void Camera::update() {
    mView = Matrix4::createLookAt(mPosition, mLookAt, mUp);
}

const Vector3& Camera::getPosition() const {
    return mPosition;
}

const Matrix4& Camera::getView() const {
    return mView;
}

const Matrix4& Camera::getProjection() const {
    return mProjection;
}

void Camera::lookAt(const Vector3& position) {
    mLookAt = position;
}

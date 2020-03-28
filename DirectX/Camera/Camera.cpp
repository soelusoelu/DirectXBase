#include "Camera.h"
#include "../Actor/PlayerActor.h"
#include "../Actor/Transform3D.h"
#include "../System/Window.h"
#include "../Utility/LevelLoader.h"

Camera::Camera() :
    mPosition(Vector3::zero),
    mLookAt(Vector3::zero),
    mUp(Vector3::up),
    mFOV(45.f),
    mNearClip(0.1f),
    mFarClip(100.f),
    mView(Matrix4::identity),
    mProjection(Matrix4::identity) {
}

Camera::~Camera() = default;

void Camera::loadProperties(const rapidjson::Value & inObj) {
    const auto& obj = inObj["camera"];
    if (obj.IsObject()) {
        JsonHelper::getVector3(obj, "position", &mPosition);
        JsonHelper::getFloat(obj, "fov", &mFOV);
        JsonHelper::getFloat(obj, "nearClip", &mNearClip);
        JsonHelper::getFloat(obj, "farClip", &mFarClip);
    }
}

void Camera::initialize() {
    calcLookAt();
    calcPerspectiveFOV(Window::width(), Window::height());
}

void Camera::update() {
    calcLookAt();
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

void Camera::calcLookAt() {
    Vector3 zaxis = Vector3::normalize(mLookAt - mPosition);
    Vector3 xaxis = Vector3::normalize(Vector3::cross(mUp, zaxis));
    Vector3 yaxis = Vector3::normalize(Vector3::cross(zaxis, xaxis));
    Vector3 trans;
    trans.x = -Vector3::dot(xaxis, mPosition);
    trans.y = -Vector3::dot(yaxis, mPosition);
    trans.z = -Vector3::dot(zaxis, mPosition);

    float temp[4][4] = {
        { xaxis.x, yaxis.x, zaxis.x, 0.f },
        { xaxis.y, yaxis.y, zaxis.y, 0.f },
        { xaxis.z, yaxis.z, zaxis.z, 0.f },
        { trans.x, trans.y, trans.z, 1.f }
    };
    mView = Matrix4(temp);
}

void Camera::calcPerspectiveFOV(float width, float height) {
    float yScale = Math::cot(mFOV / 2.f);
    float xScale = yScale * height / width;
    float temp[4][4] = {
        { xScale, 0.f, 0.f, 0.f },
        { 0.f, yScale, 0.f, 0.f },
        { 0.f, 0.f, mFarClip / (mFarClip - mNearClip), 1.f },
        { 0.f, 0.f, -mNearClip * mFarClip / (mFarClip - mNearClip), 0.f }
    };
    mProjection = Matrix4(temp);
}

#include "Camera.h"
#include "../Actor/PlayerActor.h"
#include "../Actor/Transform3D.h"
#include "../System/Game.h"

Camera::Camera() :
    mPosition(Vector3::zero),
    mLookAt(Vector3::zero),
    mUp(Vector3::up),
    mView(Matrix4::identity),
    mProjection(Matrix4::identity) {
    mPosition.set(5.f, 7.f, -14.f);
    mView = Matrix4::createLookAt(mPosition, mLookAt, mUp);
    mProjection = Matrix4::createPerspectiveFOV(45.f, Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT, 0.1f, 1000.f);
}

Camera::~Camera() = default;

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

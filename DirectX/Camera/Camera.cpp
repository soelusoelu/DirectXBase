#include "Camera.h"
#include "../Actor/PlayerActor.h"
#include "../Actor/Transform3D.h"
#include "../System/Game.h"

Camera::Camera() :
    mPosition(Vector3::zero),
    mLookAt(Vector3::zero),
    mUp(Vector3::up),
    mView(Matrix4::identity),
    mProjection(Matrix4::identity),
    mPlayer() {
    mPosition.set(5.f, 7.f, -14.f);
    mView = Matrix4::createLookAt(mPosition, mLookAt, mUp);
    mProjection = Matrix4::createPerspectiveFOV(45.f, Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT, 0.1f, 1000.f);
}

Camera::~Camera() = default;

void Camera::update() {
    //if (auto player = mPlayer.lock()) {
    //    auto t = player->transform();
    //    auto forward = t->forward();

    //    mPosition.x = -forward.x * 2.f;
    //    mPosition.y = t->getPosition().y + 0.5f;
    //    mPosition.z = -forward.z * 2.f;

    //    mLookAt.set(t->getPosition().x, t->getPosition().y + 1.f, t->getPosition().z + 5.f);
    //}
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

void Camera::setPlayer(std::shared_ptr<PlayerActor> player) {
    mPlayer = player;
}

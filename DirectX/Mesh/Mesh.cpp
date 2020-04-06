#include "Mesh.h"
#include "MeshManager.h"
#include "../Device/AssetsManager.h"

Mesh::Mesh(const std::string& fileName) :
    mMesh(Singleton<AssetsManager>::instance().createMesh(fileName)),
    mState(State::ACTIVE),
    mRadius(0.f),
    mCenter(Vector3::zero) {

    //半径と中心座標の取得
    mCenter = mMesh->getCenter();
    mRadius = mMesh->getRadius();
}

Mesh::~Mesh() = default;

std::shared_ptr<IMeshLoader> Mesh::getMeshData() const {
    return mMesh;
}

float Mesh::getRadius() const {
    return mRadius;
}

const Vector3& Mesh::getCenter() const {
    return mCenter;
}

void Mesh::destroy() {
    mState = State::DEAD;
}

void Mesh::setActive(bool value) {
    mState = (value) ? State::ACTIVE : State::NON_ACTIVE;
}

bool Mesh::getActive() const {
    return mState == State::ACTIVE;
}

bool Mesh::isDead() const {
    return mState == State::DEAD;
}

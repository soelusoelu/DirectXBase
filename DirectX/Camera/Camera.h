#pragma once

#include "../Utility/Math.h"
#include <memory>

class PlayerActor;

class Camera {
public:
    Camera();
    ~Camera();
    void update();
    const Vector3& getPosition() const;
    const Matrix4& getView() const;
    const Matrix4& getProjection() const;
    void lookAt(const Vector3& position);

private:
    Vector3 mPosition;
    Vector3 mLookAt;
    Vector3 mUp;

    Matrix4 mView;
    Matrix4 mProjection;
};


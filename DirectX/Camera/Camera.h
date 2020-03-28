#pragma once

#include "../Utility/Math.h"
#include <rapidjson/document.h>
#include <memory>

class PlayerActor;

class Camera {
public:
    Camera();
    ~Camera();
    void loadProperties(const rapidjson::Value& inObj);
    void initialize();
    void update();
    const Vector3& getPosition() const;
    const Matrix4& getView() const;
    const Matrix4& getProjection() const;
    void lookAt(const Vector3& position);

private:
    void calcLookAt();
    void calcPerspectiveFOV(float width, float height);

private:
    Vector3 mPosition;
    Vector3 mLookAt;
    Vector3 mUp;

    float mFOV;
    float mNearClip;
    float mFarClip;

    Matrix4 mView;
    Matrix4 mProjection;
};


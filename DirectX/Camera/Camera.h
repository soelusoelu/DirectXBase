#pragma once

#include "../Math/Math.h"
#include <rapidjson/document.h>
#include <memory>

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
    //�X�N���[�����W�����[���h���W�ɕϊ�
    Vector3 screenToWorldPoint(const Vector2& position, float z);

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


#pragma once

#include "Component.h"
#include "../Math/Math.h"

class GameObject;

class Camera : public Component {
public:
    Camera(std::shared_ptr<GameObject> owner);
    ~Camera();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(debugInfoList* inspect) const override;
    const Matrix4& getView() const;
    const Matrix4& getProjection() const;
    // View * Projection
    Matrix4 getViewProjection() const;
    void lookAt(const Vector3& position);
    //スクリーン座標をワールド座標に変換
    Vector3 screenToWorldPoint(const Vector2& position, float z);

private:
    void calcLookAt();
    void calcPerspectiveFOV(float width, float height);

private:
    Vector3 mLookAt;
    Vector3 mUp;

    float mFOV;
    float mNearClip;
    float mFarClip;

    Matrix4 mView;
    Matrix4 mProjection;
};


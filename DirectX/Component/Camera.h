#pragma once

#include "Component.h"
#include "../Math/Math.h"

class GameObject;

class Camera : public Component {
public:
    Camera();
    ~Camera();
    virtual void start() override;
    virtual void update() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(DebugInfoList* inspect) const override;
    const Matrix4& getView() const;
    const Matrix4& getProjection() const;
    // View * Projection
    Matrix4 getViewProjection() const;
    void lookAt(const Vector3& position);
    //スクリーン座標をワールド座標に変換
    Vector3 screenToWorldPoint(const Vector2& position, float z);
    //視錐台カリング
    //true: 視錐台の内側
    //false : 視錐台の外側
    bool viewFrustumCulling(const Vector3& pos, float radius) const;

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


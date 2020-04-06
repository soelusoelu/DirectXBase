#pragma once

#include "IMeshLoader.h"
#include "../Math/Math.h"
#include <memory>
#include <string>

class Mesh : public std::enable_shared_from_this<Mesh> {
public:
    enum class State {
        ACTIVE,
        NON_ACTIVE,
        DEAD
    };

    Mesh(const std::string& fileName);
    ~Mesh();
    std::shared_ptr<IMeshLoader> getMeshData() const;
    //半径の取得
    float getRadius() const;
    //中心座標の取得
    const Vector3& getCenter() const;
    //メッシュの破棄
    void destroy();
    void setActive(bool value);
    bool getActive() const;
    bool isDead() const;

private:
    std::shared_ptr<IMeshLoader> mMesh;
    State mState;
    float mRadius;
    Vector3 mCenter;
};

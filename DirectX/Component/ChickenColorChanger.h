#pragma once

#include "Component.h"
#include "ChickenSurface.h"
#include "FryState.h"
#include "../Math/Math.h"
#include <memory>
#include <vector>

struct Material;

class ChickenColorChanger : public Component {
    using MaterialPtr = std::shared_ptr<Material>;
    using MaterialPtrArray = std::vector<MaterialPtr>;

public:
    ChickenColorChanger();
    ~ChickenColorChanger();
    virtual void start() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(DebugInfoList* inspect) const override;
    //初期化
    void initialize();
    //更新
    void update(ChickenSurface bottomSurface, FryState bottomState);

private:
    //揚げ具合によって色を変える
    void updateColor(ChickenSurface bottomSurface, FryState bottomState);
    //指定した面のマテリアルの取得
    MaterialPtr getMaterial(ChickenSurface surface) const;

private:
    MaterialPtrArray mMaterials;
    Vector3 mInitColor;
    Vector3 mLittleFriedColor;
    Vector3 mUsuallyColor;
    Vector3 mFriedColor;
    Vector3 mBurntColor;
};

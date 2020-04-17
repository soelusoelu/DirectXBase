#pragma once

#include "Component.h"
#include "../Math/Math.h"
#include <memory>
#include <vector>

class Time;
struct Material;

class ChickenFry : public Component {
    using MaterialPtr = std::shared_ptr<Material>;
    using MaterialPtrArray = std::vector<MaterialPtr>;

    enum class Surface {
        RIGHT,
        FORE,
        LEFT,
        UP,
        BOTTOM,
        BACK,

        NUM_SURFACE
    };

public:
    ChickenFry(std::shared_ptr<GameObject> owner);
    ~ChickenFry();
    virtual void start() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(DebugInfoList* inspect) const override;
    //初期化
    void initialize();
    //FriedChickenComponentにより毎フレーム更新
    void update();
    //すべての面を揚げ終わったか
    bool isFriedAllSurfaces(float level) const;
    //面の数の取得
    int getNumSurface() const;
    //指定した面の揚げ時間の取得
    float getFryRate(unsigned surfaceIndex) const;

private:
    //下の面を決定する
    void bottomSurface();
    //下の面を揚げる
    void frying();
    //揚げ具合によって色を変える
    void changeFryedColor();
    //指定した面のマテリアルの取得
    MaterialPtr getMaterial(Surface surface) const;

private:
    std::vector<std::unique_ptr<Time>> mFryTimer;
    MaterialPtrArray mMaterials;
    Surface mCurrentBottomSurface;
    Vector3 mInitColor;
    Vector3 mFryedColor;
    Vector3 mBurntColor;
};

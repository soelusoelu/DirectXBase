#pragma once

#include "Component.h"
#include <rapidjson/document.h>
#include <memory>

class GameObject;

class HitPointComponent : public Component {
public:
    HitPointComponent();
    ~HitPointComponent();
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(DebugInfoList* inspect) const override;
    void takeDamage(int damage);
    void takeHeal(int heal);
    void setHP(int hp, bool isChangeMax = true);
    //現在HPの取得
    int hp() const;
    //最大HPに対しての現在HPの比率(0 ～ 1)
    float hpRate() const;

private:
    int mHP;
    int mMaxHP;
};


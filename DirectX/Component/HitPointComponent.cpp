#include "HitPointComponent.h"
#include "../Actor/Actor.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/Math.h"
#include <string>

HitPointComponent::HitPointComponent(std::shared_ptr<Actor> onwer) :
    Component(onwer, "HitPointComponent"),
    mHP(0),
    mMaxHP(0) {
}

HitPointComponent::~HitPointComponent() = default;

void HitPointComponent::loadProperties(const rapidjson::Value & inObj) {
    Component::loadProperties(inObj);

    JsonHelper::getInt(inObj, "HP", &mHP);
    JsonHelper::getInt(inObj, "maxHP", &mMaxHP);
}

void HitPointComponent::takeDamage(int damage) {
    mHP -= damage;
    mHP = Math::Max<int>(mHP, 0);
}

void HitPointComponent::takeHeal(int heal) {
    mHP += heal;
    mHP = Math::Min<int>(mHP, mMaxHP);
}

void HitPointComponent::setHP(int hp, bool isChangeMax) {
    mHP = hp;
    if (isChangeMax) {
        mMaxHP = hp;
    } else {
        mHP = Math::Min<int>(mHP, mMaxHP);
    }
}

int HitPointComponent::hp() const {
    return mHP;
}

float HitPointComponent::hpRate() const {
    return static_cast<float>(mHP) / static_cast<float>(mMaxHP);
}

#include "ChickenFry.h"
#include "ChickenMeshComponent.h"
#include "ComponentManager.h"
#include "../DebugLayer/Debug.h"
#include "../DebugLayer/Inspector.h"
#include "../Device/Time.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"
#include "../Mesh/Material.h"
#include "../Utility/LevelLoader.h"
#include <cassert>

ChickenFry::ChickenFry(std::shared_ptr<GameObject> owner) :
    Component(owner, "ChickenFry"),
    mCurrentBottomSurface(Surface::BOTTOM),
    mInitColor(Vector3::zero),
    mFryedColor(Vector3::zero),
    mBurntColor(Vector3::zero) {
}

ChickenFry::~ChickenFry() = default;

void ChickenFry::start() {
    auto mesh = owner()->componentManager()->getComponent<ChickenMeshComponent>();
    if (mesh) {
        for (size_t i = 0; i < getNumSurface(); i++) {
            //Surfaceと合わせてある
            //最初のマテリアルは使われてないからプラス1
            mMaterials.emplace_back(mesh->getMaterial(i + 1));
        }
    }

    for (size_t i = 0; i < getNumSurface(); i++) {
        mFryTimer.emplace_back(std::make_unique<Time>(10.f));
    }

    initialize();
}

void ChickenFry::onUpdateWorldTransform() {
    bottomSurface();
}

void ChickenFry::loadProperties(const rapidjson::Value& inObj) {
    Component::loadProperties(inObj);

    JsonHelper::getVector3(inObj, "initColor", &mInitColor);
    JsonHelper::getVector3(inObj, "fryedColor", &mFryedColor);
    JsonHelper::getVector3(inObj, "burntColor", &mBurntColor);
}

void ChickenFry::drawDebugInfo(DebugInfoList* inspect) const {
    Component::drawDebugInfo(inspect);

    DebugInfo info;
    info.first = "InitColor";
    info.second = InspectHelper::vector3ToString(mInitColor);
    inspect->emplace_back(info);
    info.first = "FryedColor";
    info.second = InspectHelper::vector3ToString(mFryedColor);
    inspect->emplace_back(info);
    info.first = "BurntColor";
    info.second = InspectHelper::vector3ToString(mBurntColor);
    inspect->emplace_back(info);

    info.first = "currentBottomSurface";
    if (mCurrentBottomSurface == Surface::UP) {
        info.second = "UP";
    } else if (mCurrentBottomSurface == Surface::BOTTOM) {
        info.second = "BOTTOM";
    } else if (mCurrentBottomSurface == Surface::LEFT) {
        info.second = "LEFT";
    } else if (mCurrentBottomSurface == Surface::RIGHT) {
        info.second = "RIGHT";
    } else if (mCurrentBottomSurface == Surface::FORE) {
        info.second = "FORE";
    } else if (mCurrentBottomSurface == Surface::BACK) {
        info.second = "BACK";
    }
    inspect->emplace_back(info);
}

void ChickenFry::initialize() {
    //揚げる面の初期化
    for (auto&& timer : mFryTimer) {
        timer->reset();
    }
    for (auto&& mat : mMaterials) {
        mat->diffuse = mInitColor;
    }
}

void ChickenFry::update() {
    frying();
    changeFryedColor();
}

bool ChickenFry::isFriedAllSurfaces(float level) const {
    bool result = true;
    for (size_t i = 0; i < getNumSurface(); i++) {
        if (mFryTimer[i]->rate() < level) {
            result = false;
            break;
        }
    }

    return result;
}

int ChickenFry::getNumSurface() const {
    return static_cast<int>(Surface::NUM_SURFACE);
}

float ChickenFry::getFryRate(unsigned surfaceIndex) const {
    return mFryTimer[surfaceIndex]->rate();
}

void ChickenFry::bottomSurface() {
    auto dir = Vector3::transform(Vector3::down, owner()->transform()->getRotation());
    auto x = Math::cos(dir.x);
    auto y = Math::sin(dir.y);
    static const float SIN_COS_45 = Math::sin(45.f);
    if (dir.y <= -SIN_COS_45) {
        mCurrentBottomSurface = Surface::BOTTOM;
    } else if (dir.y > SIN_COS_45) {
        mCurrentBottomSurface = Surface::UP;
    } else if (dir.x <= -SIN_COS_45) {
        mCurrentBottomSurface = Surface::RIGHT;
    } else if (dir.x > SIN_COS_45) {
        mCurrentBottomSurface = Surface::LEFT;
    } else if (dir.z <= SIN_COS_45) {
        mCurrentBottomSurface = Surface::FORE;
    } else if (dir.z > SIN_COS_45) {
        mCurrentBottomSurface = Surface::BACK;
    } else {
        Debug::logError("No bottom Surface!");
    }
}

void ChickenFry::frying() {
    mFryTimer[static_cast<int>(mCurrentBottomSurface)]->update();
}

void ChickenFry::changeFryedColor() {
    auto rate = mFryTimer[static_cast<int>(mCurrentBottomSurface)]->rate();
    auto a = mInitColor;
    auto b = mFryedColor;
    if (rate > 1.f) {
        rate -= 1.f;
        a = mFryedColor;
        b = mBurntColor;
    }
    if (rate > 1.f) {
        rate = 1.f;
    }
    auto color = Vector3::lerp(a, b, rate);
    getMaterial(mCurrentBottomSurface)->diffuse = color;
}

std::shared_ptr<Material> ChickenFry::getMaterial(Surface surface) const {
    int index = static_cast<int>(surface);
    assert(0 <= index && index < getNumSurface());
    return mMaterials[index];
}

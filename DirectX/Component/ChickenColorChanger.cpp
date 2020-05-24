#include "ChickenColorChanger.h"
#include "ChickenMeshComponent.h"
#include "ComponentManager.h"
#include "../DebugLayer/Debug.h"
#include "../GameObject/GameObject.h"
#include "../Mesh/Material.h"
#include "../Utility/LevelLoader.h"

ChickenColorChanger::ChickenColorChanger() :
    Component(),
    mInitColor(Vector3::zero),
    mLittleFriedColor(Vector3::zero),
    mUsuallyColor(Vector3::zero),
    mFriedColor(Vector3::zero),
    mBurntColor(Vector3::zero) {
}

ChickenColorChanger::~ChickenColorChanger() = default;

void ChickenColorChanger::start() {
    auto mesh = owner()->componentManager()->getComponent<ChickenMeshComponent>();
    if (mesh) {
        for (size_t i = 0; i < static_cast<int>(ChickenSurface::NUM_SURFACE); i++) {
            //Surfaceと合わせてある
            mMaterials.emplace_back(mesh->getMaterial(i));
        }
    }

    initialize();
}

void ChickenColorChanger::loadProperties(const rapidjson::Value& inObj) {
    Component::loadProperties(inObj);

    JsonHelper::getVector3(inObj, "initColor", &mInitColor);
    JsonHelper::getVector3(inObj, "littleFriedColor", &mLittleFriedColor);
    JsonHelper::getVector3(inObj, "usuallyColor", &mUsuallyColor);
    JsonHelper::getVector3(inObj, "friedColor", &mFriedColor);
    JsonHelper::getVector3(inObj, "burntColor", &mBurntColor);
}

void ChickenColorChanger::drawDebugInfo(DebugInfoList* inspect) const {
    Component::drawDebugInfo(inspect);

    DebugInfo info;
    info.first = "InitColor";
    info.second = mInitColor;
    inspect->emplace_back(info);
    info.first = "LittleFriedColor";
    info.second = mLittleFriedColor;
    inspect->emplace_back(info);
    info.first = "UsuallyColor";
    info.second = mUsuallyColor;
    inspect->emplace_back(info);
    info.first = "FriedColor";
    info.second = mFriedColor;
    inspect->emplace_back(info);
    info.first = "BurntColor";
    info.second = mBurntColor;
    inspect->emplace_back(info);
}

void ChickenColorChanger::initialize() {
    for (auto&& mat : mMaterials) {
        mat->diffuse = mInitColor;
    }
}

void ChickenColorChanger::update(ChickenSurface bottomSurface, FryState bottomState) {
    updateColor(bottomSurface, bottomState);
}

void ChickenColorChanger::updateColor(ChickenSurface bottomSurface, FryState bottomState) {
    auto color = ColorPalette::black;

    if (bottomState == FryState::NOT_FRIED) {
        color = mInitColor;
    } else if (bottomState == FryState::LITTLE_BAD) {
        color = mLittleFriedColor;
    } else if (bottomState == FryState::USUALLY) {
        color = mUsuallyColor;
    } else if (bottomState == FryState::GOOD) {
        color = mFriedColor;
    } else if (bottomState == FryState::BAD) {
        color = mBurntColor;
    } else {
        Debug::logError(getComponentName() + ": Invalid state");
    }

    getMaterial(bottomSurface)->diffuse = color;
}

std::shared_ptr<Material> ChickenColorChanger::getMaterial(ChickenSurface surface) const {
    return mMaterials[static_cast<int>(surface)];
}

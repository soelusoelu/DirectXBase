﻿#include "ChickenColorChanger.h"
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
    JsonHelper::getVector3(inObj, "initColor", &mInitColor);
    JsonHelper::getVector3(inObj, "littleFriedColor", &mLittleFriedColor);
    JsonHelper::getVector3(inObj, "usuallyColor", &mUsuallyColor);
    JsonHelper::getVector3(inObj, "friedColor", &mFriedColor);
    JsonHelper::getVector3(inObj, "burntColor", &mBurntColor);
}

void ChickenColorChanger::drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const {
    inspect->emplace_back("InitColor", mInitColor);
    inspect->emplace_back("LittleFriedColor", mLittleFriedColor);
    inspect->emplace_back("UsuallyColor", mUsuallyColor);
    inspect->emplace_back("FriedColor", mFriedColor);
    inspect->emplace_back("BurntColor", mBurntColor);
}

void ChickenColorChanger::initialize() {
    for (auto&& mat : mMaterials) {
        mat->diffuse = mInitColor;
    }
}

void ChickenColorChanger::update(ChickenSurface bottomSurface, FryState bottomState) {
    updateColor(bottomSurface, bottomState);
}

void ChickenColorChanger::changeAllSurfaces(FryState state) {
    auto color = getColor(state);
    for (auto&& mat : mMaterials) {
        mat->diffuse = color;
    }
}

void ChickenColorChanger::updateColor(ChickenSurface bottomSurface, FryState bottomState) {
    getMaterial(bottomSurface)->diffuse = getColor(bottomState);
}

Vector3 ChickenColorChanger::getColor(FryState state) {
    auto color = ColorPalette::black;

    if (state == FryState::NOT_FRIED) {
        color = mInitColor;
    } else if (state == FryState::LITTLE_BAD) {
        color = mLittleFriedColor;
    } else if (state == FryState::USUALLY) {
        color = mUsuallyColor;
    } else if (state == FryState::GOOD) {
        color = mFriedColor;
    } else if (state == FryState::BAD) {
        color = mBurntColor;
    } else {
        Debug::logError(getComponentName() + ": Invalid state");
    }

    return color;
}

std::shared_ptr<Material> ChickenColorChanger::getMaterial(ChickenSurface surface) const {
    return mMaterials[static_cast<int>(surface)];
}

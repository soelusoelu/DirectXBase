#include "ChickenFry.h"
#include "ChickenMeshComponent.h"
#include "ComponentManager.h"
#include "../DebugLayer/Debug.h"
#include "../DebugLayer/Inspector.h"
#include "../Device/Random.h"
#include "../Device/Time.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Transform3D.h"
#include "../Mesh/Material.h"
#include "../Utility/LevelLoader.h"
#include <cassert>

ChickenFry::ChickenFry(std::shared_ptr<GameObject> owner) :
    Component(owner, "ChickenFry"),
    mCurrentBottomSurface(Surface::BOTTOM),
    mEasySurface(Surface::BOTTOM),
    mHardSurface(Surface::BOTTOM),
    mInitColor(Vector3::zero),
    mFryedColor(Vector3::zero),
    mBurntColor(Vector3::zero),
    mUsually(getNumFryState() - 1),
    mEasy(getNumFryState() - 1),
    mHard(getNumFryState() - 1),
    mBurntTime(0.f),
    mEasyBurntTime(0.f),
    mHardBurntTime(0.f) {
}

ChickenFry::~ChickenFry() = default;

void ChickenFry::awake() {
    for (size_t i = 0; i < getNumFryState() - 1; i++) {
        mBurntTime += mUsually[i];
        mEasyBurntTime += mEasy[i];
        mHardBurntTime += mHard[i];
    }

    for (size_t i = 0; i < getNumSurface(); i++) {
        mFryTimer.emplace_back(std::make_shared<Time>(mBurntTime));
    }
}

void ChickenFry::start() {
    auto mesh = owner()->componentManager()->getComponent<ChickenMeshComponent>();
    if (mesh) {
        for (size_t i = 0; i < getNumSurface(); i++) {
            //Surfaceと合わせてある
            //最初のマテリアルは使われてないからプラス1
            mMaterials.emplace_back(mesh->getMaterial(i + 1));
        }
    }

    initialize();
}

void ChickenFry::onUpdateWorldTransform() {
    bottomSurface();
}

void ChickenFry::loadProperties(const rapidjson::Value & inObj) {
    Component::loadProperties(inObj);

    JsonHelper::getVector3(inObj, "initColor", &mInitColor);
    JsonHelper::getVector3(inObj, "fryedColor", &mFryedColor);
    JsonHelper::getVector3(inObj, "burntColor", &mBurntColor);

    JsonHelper::getFloat(inObj, "notFriedToLittleBadTimer", &mUsually[0]);
    JsonHelper::getFloat(inObj, "littleBadToUsuallyTimer", &mUsually[1]);
    JsonHelper::getFloat(inObj, "usuallyToGoodTimer", &mUsually[2]);
    JsonHelper::getFloat(inObj, "goodToBadTimer", &mUsually[3]);

    JsonHelper::getFloat(inObj, "easyNotFriedToLittleBadTimer", &mEasy[0]);
    JsonHelper::getFloat(inObj, "easyLittleBadToUsuallyTimer", &mEasy[1]);
    JsonHelper::getFloat(inObj, "easyUsuallyToGoodTimer", &mEasy[2]);
    JsonHelper::getFloat(inObj, "easyGoodToBadTimer", &mEasy[3]);

    JsonHelper::getFloat(inObj, "hardNotFriedToLittleBadTimer", &mHard[0]);
    JsonHelper::getFloat(inObj, "hardLittleBadToUsuallyTimer", &mHard[1]);
    JsonHelper::getFloat(inObj, "hardUsuallyToGoodTimer", &mHard[2]);
    JsonHelper::getFloat(inObj, "hardGoodToBadTimer", &mHard[3]);
}

void ChickenFry::drawDebugInfo(DebugInfoList * inspect) const {
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

    info.first = "CurrentBottomSurface";
    info.second = surfaceToString(mCurrentBottomSurface);
    inspect->emplace_back(info);

    info.first = "EasySurface";
    info.second = surfaceToString(mEasySurface);
    inspect->emplace_back(info);

    info.first = "HardSurface";
    info.second = surfaceToString(mHardSurface);
    inspect->emplace_back(info);
}

void ChickenFry::initialize() {
    //揚げる面の初期化
    for (auto&& timer : mFryTimer) {
        timer->setLimitTime(mBurntTime);
        timer->reset();
    }
    for (auto&& mat : mMaterials) {
        mat->diffuse = mInitColor;
    }

    choiceEasyAndHardSurface();
}

void ChickenFry::update() {
    frying();
    changeFryedColor();
}

bool ChickenFry::isFriedAllSurfaces() const {
    bool result = true;
    for (size_t i = 0; i < getNumSurface(); i++) {
        if (getFryState(i) != FryState::BAD) {
            result = false;
            break;
        }
    }

    return result;
}

int ChickenFry::getNumSurface() const {
    return static_cast<int>(Surface::NUM_SURFACE);
}

FryState ChickenFry::getFryState(unsigned surfaceIndex) const {
    std::vector<float> timer;
    if (surfaceIndex == static_cast<unsigned>(mEasySurface)) {
        timer = mEasy;
    } else if (surfaceIndex == static_cast<unsigned>(mHardSurface)) {
        timer = mHard;
    } else {
        timer = mUsually;
    }

    FryState state = FryState::NOT_FRIED;
    auto current = mFryTimer[surfaceIndex]->currentTime();
    float sum = 0.f;
    for (size_t i = 0; i < getNumFryState() - 1; i++) {
        sum += timer[i];
        if (current < sum) {
            state = static_cast<FryState>(i);
            break;
        }
    }
    if (current > sum) {
        state = FryState::BAD;
    }

    return state;
}

void ChickenFry::choiceEasyAndHardSurface() {
    auto easySurface = Random::randomRange(0, getNumSurface() - 1);
    int hardSurface;
    do {
        hardSurface = Random::randomRange(0, getNumSurface() - 1);
    } while (hardSurface == easySurface);

    mEasySurface = static_cast<Surface>(easySurface);
    mHardSurface = static_cast<Surface>(hardSurface);
    mFryTimer[static_cast<int>(mEasySurface)]->setLimitTime(mEasyBurntTime);
    mFryTimer[static_cast<int>(mHardSurface)]->setLimitTime(mHardBurntTime);
}

void ChickenFry::bottomSurface() {
    auto dir = Vector3::transform(Vector3::down, owner()->transform()->getRotation());
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

int ChickenFry::getNumFryState() const {
    return static_cast<int>(FryState::NUM_STATE);
}

std::string ChickenFry::surfaceToString(Surface surface) const {
    std::string str;

    if (surface == Surface::UP) {
        str = "UP";
    } else if (surface == Surface::BOTTOM) {
        str = "BOTTOM";
    } else if (surface == Surface::LEFT) {
        str = "LEFT";
    } else if (surface == Surface::RIGHT) {
        str = "RIGHT";
    } else if (surface == Surface::FORE) {
        str = "FORE";
    } else if (surface == Surface::BACK) {
        str = "BACK";
    }

    return str;
}

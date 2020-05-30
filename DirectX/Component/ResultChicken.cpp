#include "ResultChicken.h"
#include "ComponentManager.h"
#include "ResultRank.h"
#include "../Device/Time.h"
#include "../GameObject/GameObject.h"

ResultChicken::ResultChicken() :
    Component(),
    mRank(nullptr),
    mFallToFadeTimer(std::make_unique<Time>(5.f)),
    mState(State::FALL),
    mScore(0)
{
}

ResultChicken::~ResultChicken() = default;

void ResultChicken::start() {
    mRank = owner()->componentManager()->getComponent<ResultRank>();
    mRank->initialize(mScore);
}

void ResultChicken::update() {
    if (mState == State::FALL) {
        mFallToFadeTimer->update();
        if (mFallToFadeTimer->isTime()) {
            mState = State::FADE;
        }
    } else if (mState == State::FADE) {

    } else if (mState == State::RESULT) {
        mRank->setActive(true);
    }
}

void ResultChicken::initialize(int score) {
    mScore = score;
}

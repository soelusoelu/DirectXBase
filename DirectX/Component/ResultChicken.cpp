#include "ResultChicken.h"
#include "ComponentManager.h"
#include "Fade.h"
#include "ResultChickenManager.h"
#include "ResultRank.h"
#include "SoundComponent.h"
#include "Text.h"
#include "../Device/DrawString.h"
#include "../Device/Time.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectFactory.h"
#include "../Input/Input.h"
#include "../Input/JoyPad.h"
#include "../Input/Keyboard.h"
#include "../System/Window.h"
#include "../Utility/LevelLoader.h"
#include "../Utility/StringUtil.h"

ResultChicken::ResultChicken() :
    Component(),
    mRank(nullptr),
    mResultChickenManager(nullptr),
    mFade(nullptr),
    mFallToFadeTimer(std::make_unique<Time>()),
    mState(State::FALL),
    mScore(0),
    mIsSleepedFirstFrame(false) {
}

ResultChicken::~ResultChicken() = default;

void ResultChicken::awake() {
    auto rr = GameObjectCreater::create("ResultRank");
    mRank = rr->componentManager()->getComponent<ResultRank>();
    mResultChickenManager = GameObjectCreater::create("ResultChickenManager");
    auto fadeObj = GameObjectCreater::create("Fade");
    mFade = fadeObj->componentManager()->getComponent<Fade>();
}

void ResultChicken::start() {
    mRank->initialize(mScore);
    auto rcm = mResultChickenManager->componentManager()->getComponent<ResultChickenManager>();
    rcm->createChickensFromRank(mRank->getRank());

    mText = owner()->componentManager()->getComponent<Text>();
    mText->setText(StringUtil::intToString(mScore));
}

void ResultChicken::update() {
    //最初の1フレームは休む(無理やりバグ回避)
    if (!mIsSleepedFirstFrame) {
        mIsSleepedFirstFrame = true;
        return;
    }

    if (mState == State::FALL) {
        fallUpdate();
    } else if (mState == State::FADE) {
        fadeUpdate();
    } else if (mState == State::RESULT) {

    }

    auto isEnd = Input::joyPad()->getEnter();
#ifdef _DEBUG
    if (!isEnd) {
        isEnd = Input::keyboard()->getEnter();
    }
#endif // _DEBUG
    if (isEnd && !isResult()) {
        skipPerform();
    }
}

void ResultChicken::loadProperties(const rapidjson::Value & inObj) {
    float timer;
    if (JsonHelper::getFloat(inObj, "fallToFadeTimer", &timer)) {
        mFallToFadeTimer->setLimitTime(timer);
    }
}

void ResultChicken::initialize(int score) {
    mScore = score;
}

bool ResultChicken::isResult() const {
    return mState == State::RESULT;
}

void ResultChicken::fallUpdate() {
    mFallToFadeTimer->update();
    if (mFallToFadeTimer->isTime()) {
        mFade->fadeOut();
        mState = State::FADE;
    }
}

void ResultChicken::fadeUpdate() {
    if (mFade->isFadeOutEnded()) {
        mResultChickenManager->destroy();
        mResultChickenManager.reset();
        mRank->setActive(true);
        mText->setActive(true);
        mFade->fadeIn();
    }
    if (mFade->isFadeInEnded()) {
        owner()->componentManager()->getComponent<SoundComponent>()->playSE();
        mState = State::RESULT;
    }
}

void ResultChicken::skipPerform() {
    if (mResultChickenManager) {
        mResultChickenManager->destroy();
        mResultChickenManager.reset();
    }
    mRank->setActive(true);
    mText->setActive(true);
    mFade->reset();
    owner()->componentManager()->getComponent<SoundComponent>()->playSE();
    mState = State::RESULT;
}

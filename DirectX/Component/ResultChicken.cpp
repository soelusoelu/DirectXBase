#include "ResultChicken.h"
#include "ComponentManager.h"
#include "Fade.h"
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
    mScore(0) {
}

ResultChicken::~ResultChicken() = default;

void ResultChicken::start() {
    auto rr = GameObjectCreater::create("ResultRank");
    mRank = rr->componentManager()->getComponent<ResultRank>();
    mRank->initialize(mScore);
    mResultChickenManager = GameObjectCreater::create("ResultChickenManager");
    auto fadeObj = GameObjectCreater::create("Fade");
    mFade = fadeObj->componentManager()->getComponent<Fade>();

    mText = owner()->componentManager()->getComponent<Text>();
    auto scoreStr = StringUtil::intToString(mScore);
    auto textPosX = Window::standardWidth() / 2.f;
    textPosX -= DrawString::WIDTH * mText->getScale().x * scoreStr.length() / 2.f;
    mText->setPosition(Vector2(textPosX, mText->getPosition().y));
    mText->setText(StringUtil::intToString(mScore));
}

void ResultChicken::update() {
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

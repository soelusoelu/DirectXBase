#include "ChickenScoreConnection.h"
#include "../Component/ComponentManager.h"
#include "../Component/FriedChickenComponent.h"
#include "../Component/Score.h"
#include "../GameObject/GameObject.h"

ChickenScoreConnection::ChickenScoreConnection() :
    mChicken(nullptr),
    mScore(nullptr) {
}

ChickenScoreConnection::~ChickenScoreConnection() = default;

void ChickenScoreConnection::connect() {
    auto c = mChicken->componentManager()->getComponent<FriedChickenComponent>();
    if (c->successFrying()) {
        mScore->addScore(10);
    }
}

void ChickenScoreConnection::setChicken(const GameObjectPtr chicken) {
    mChicken = chicken;
}

void ChickenScoreConnection::setScore(const GameObjectPtr score) {
    mScore = score->componentManager()->getComponent<Score>();
}

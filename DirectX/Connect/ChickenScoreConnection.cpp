#include "ChickenScoreConnection.h"
#include "../UI/Score.h"

ChickenScoreConnection::ChickenScoreConnection() :
    mChicken(nullptr),
    mScore(nullptr) {
}

ChickenScoreConnection::~ChickenScoreConnection() = default;

void ChickenScoreConnection::connect() {
    //mScore->addScore(1);
}

void ChickenScoreConnection::setChicken(const ActorPtr chicken) {
    mChicken = chicken;
}

void ChickenScoreConnection::setScore(const ScorePtr score) {
    mScore = score;
}

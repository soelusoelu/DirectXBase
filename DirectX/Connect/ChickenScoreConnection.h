#pragma once

#include <memory>

class GameObject;
class Score;

class ChickenScoreConnection {
    using GameObjectPtr = std::shared_ptr<GameObject>;
    using ScorePtr = std::shared_ptr<Score>;

public:
    ChickenScoreConnection();
    ~ChickenScoreConnection();
    void connect();
    void setChicken(const GameObjectPtr chicken);
    void setScore(const GameObjectPtr score);

private:
    GameObjectPtr mChicken;
    ScorePtr mScore;
};

#pragma once

#include <memory>

class FriedChicken;
class Score;

class ChickenScoreConnection {
    using ChickenPtr = std::shared_ptr<FriedChicken>;
    using ScorePtr = std::shared_ptr<Score>;

public:
    ChickenScoreConnection();
    ~ChickenScoreConnection();
    void connect();
    void setChicken(const ChickenPtr chicken);
    void setScore(const ScorePtr score);

private:
    ChickenPtr mChicken;
    ScorePtr mScore;
};

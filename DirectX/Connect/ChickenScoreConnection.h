#pragma once

#include <memory>

class Actor;
class Score;

class ChickenScoreConnection {
    using ActorPtr = std::shared_ptr<Actor>;
    using ScorePtr = std::shared_ptr<Score>;

public:
    ChickenScoreConnection();
    ~ChickenScoreConnection();
    void connect();
    void setChicken(const ActorPtr chicken);
    void setScore(const ScorePtr score);

private:
    ActorPtr mChicken;
    ScorePtr mScore;
};

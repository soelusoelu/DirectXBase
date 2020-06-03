#pragma once

#include "Component.h"
#include "Rank.h"
#include <list>
#include <memory>

class GameObject;

class ResultChickenManager : public Component {
    using GameObjectPtr = std::shared_ptr<GameObject>;
    using GameObjectPtrList = std::list<GameObjectPtr>;

public:
    ResultChickenManager();
    ~ResultChickenManager();
    virtual void finalize() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;
    void createChickensFromRank(Rank rank);

private:
    GameObjectPtrList mChickens;
    int mLowNum;
    int mMiddleNum;
    int mHighNum;
    int mSuperNum;
};


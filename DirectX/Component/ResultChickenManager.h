#pragma once

#include "Component.h"
#include <list>
#include <memory>

class GameObject;

class ResultChickenManager : public Component {
    using GameObjectPtr = std::shared_ptr<GameObject>;
    using GameObjectPtrList = std::list<GameObjectPtr>;

public:
    ResultChickenManager();
    ~ResultChickenManager();
    virtual void awake() override;
    virtual void finalize() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;

private:
    GameObjectPtrList mChickens;
    int mFallsNum;
};


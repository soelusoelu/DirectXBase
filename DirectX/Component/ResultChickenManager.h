#pragma once

#include "Component.h"
#include <list>
#include <memory>

class ResultChickenFall;

class ResultChickenManager : public Component {
    using ChickenFallPtr = std::shared_ptr<ResultChickenFall>;
    using ChickenFallPtrList = std::list<ChickenFallPtr>;

public:
    ResultChickenManager();
    ~ResultChickenManager();
    virtual void awake() override;
    virtual void loadProperties(const rapidjson::Value& inObj) override;
    virtual void drawDebugInfo(ComponentDebug::DebugInfoList* inspect) const override;

private:
    ChickenFallPtrList mChickens;
    int mFallsNum;
};


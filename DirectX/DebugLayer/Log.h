#pragma once

#include "../Utility/Math.h"
#include <rapidjson/document.h>
#include <list>
#include <memory>
#include <string>
#include <utility>

class DrawString;

class Log {
public:
    Log();
    ~Log();
    void loadProperties(const rapidjson::Value& inObj);
    void initialize();
    void update();
    void log(const std::string& message);
    void logError(const std::string& message);
    void logWarning(const std::string& message);
    void drawLogs(DrawString* drawString) const;

private:
    void adjustCapacity();

private:
    //表示する文字列と色
    std::list<std::pair<std::string, Vector3>> mLogs;
    //文字のスケール
    Vector2 mScale;
    //画面に表示するログの行数
    int mNumRowsToDisplay;
};

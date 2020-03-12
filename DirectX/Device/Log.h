#pragma once

#include "../Utility/Math.h"
#include <list>
#include <memory>
#include <string>
#include <utility>

class DrawString;

class Log {
public:
    Log(std::shared_ptr<DrawString> drawString);
    ~Log();
    void log(const std::string& message);
    void logError(const std::string& message);
    void logWarning(const std::string& message);
    void drawLogs();

private:
    void adjustCapacity();

private:
    std::shared_ptr<DrawString> mDrawString;
    //表示する文字列と色
    std::list<std::pair<std::string, Vector3>> mLogs;
    static const Vector2 SCALE;
    static const int NUM_ROWS_TO_DISPLAY;
};

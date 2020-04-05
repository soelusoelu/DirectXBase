#pragma once

#include "../Math/Math.h"
#include <rapidjson/document.h>
#include <string>

class DrawString;
class FixedDebugInformation;
class Hierarchy;
class Inspector;
class Log;

class DebugUtility {
    DebugUtility() = delete;
    ~DebugUtility() = delete;

public:
    static void create();
    static void loadProperties(const rapidjson::Value& inObj);
    static void initialize();
    static void finalize();
    static void update();
    //messageを新規ウィンドウに表示
    //重大なバグを表示する用
    static void windowMessage(const std::string& message);
    static void draw(const Matrix4& proj);
    //DrawStringが保持する文字列をすべて削除
    static void drawStringClear();
    static Log* log();
    static FixedDebugInformation* fixedDebugInfo();
    static Hierarchy* hierarchy();
    static Inspector* inspector();

private:
    static DrawString* mDrawString;
    static Log* mLog;
    static FixedDebugInformation* mFixedDebugInfo;
    static Hierarchy* mHierarchy;
    static Inspector* mInspector;
};

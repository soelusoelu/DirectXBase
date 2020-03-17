#pragma once

#include "../Utility/Math.h"
#include <rapidjson/document.h>
#include <memory>
#include <string>

class DrawString;
class FixedDebugInformation;
class Hierarchy;
class Inspector;
class Log;
class Renderer;

class Debug {
public:
    static void create();
    static void loadProperties(const rapidjson::Value& inObj);
    static void initialize(std::shared_ptr<Renderer> renderer);
    static void finalize();
    static void windowMessage(const std::string& message);
    static void draw(const Matrix4& proj);
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

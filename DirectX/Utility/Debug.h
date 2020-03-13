#pragma once

#include <memory>
#include <string>

class FixedDebugInformation;
class Inspector;
class Log;
class Renderer;

class Debug {
public:
    static void initialize(std::shared_ptr<Renderer> renderer);
    static void finalize();
    static void windowMessage(const std::string& message);
    static Log* log();
    static FixedDebugInformation* fixedDebugInfo();
    static Inspector* inspector();

private:
    static Log* mLog;
    static FixedDebugInformation* mFixedDebugInfo;
    static Inspector* mInspector;
};

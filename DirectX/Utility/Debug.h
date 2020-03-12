#pragma once

#include <memory>
#include <string>

class FixedDebugInformation;
class Log;
class Renderer;

class Debug {
public:
    static void initialize(std::shared_ptr<Renderer> renderer);
    static void finalize();
    static void windowMessage(const std::string& message);
    static Log* log();
    static FixedDebugInformation* fixedDebugInfo();

private:
    static Log* mLog;
    static FixedDebugInformation* mFixedDebugInfo;
};

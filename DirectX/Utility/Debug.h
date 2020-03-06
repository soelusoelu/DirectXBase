#pragma once

#include <memory>
#include <string>

class Log;
class Renderer;

class Debug {
public:
    static void initialize(std::shared_ptr<Renderer> renderer);
    static void finalize();
    static void windowMessage(const std::string& message);
    static Log* log();

private:
    static Log* mLog;
};

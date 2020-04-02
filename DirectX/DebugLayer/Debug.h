#pragma once

#include <string>

class Debug {
    Debug() = delete;
    ~Debug() = delete;

public:
    static void windowMessage(const std::string& message);
    static void log(const std::string& message);
    static void logError(const std::string& message);
    static void logWarning(const std::string& message);
};

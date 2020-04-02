#include "Debug.h"
#include "DebugUtility.h"
#include "Log.h"

void Debug::windowMessage(const std::string& message) {
    DebugUtility::windowMessage(message);
}

void Debug::log(const std::string& message) {
    DebugUtility::log()->log(message);
}

void Debug::logError(const std::string& message) {
    DebugUtility::log()->logError(message);
}

void Debug::logWarning(const std::string& message) {
    DebugUtility::log()->logWarning(message);
}

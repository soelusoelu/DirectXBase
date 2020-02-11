#include "StringUtil.h"

std::string StringUtil::spritFirst(const std::string& string, const char delimiter) {
    return string.substr(0, string.find_first_of(delimiter));
}

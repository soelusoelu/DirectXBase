#include "StringUtil.h"

std::string StringUtil::spritFirst(const std::string& string, const char delimiter) {
    return string.substr(0, string.find_first_of(delimiter));
}

wchar_t* StringUtil::charToWchar(const char* src) {
    size_t len = 0;
    wchar_t dst[256];
    mbstowcs_s(&len, dst, 256, src, _TRUNCATE);

    return dst;
}

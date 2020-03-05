#include "StringUtil.h"
#include <locale.h>

std::string StringUtil::spritFirst(const std::string& string, const char delimiter) {
    return string.substr(0, string.find_first_of(delimiter));
}

wchar_t* StringUtil::charToWchar(const char* src) {
    size_t len = 0;
    wchar_t dst[256];
    setlocale(LC_ALL, "japanese");
    mbstowcs_s(&len, dst, 256, src, _TRUNCATE);

    return dst;
}

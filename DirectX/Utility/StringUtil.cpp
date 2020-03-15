#include "StringUtil.h"
#include <locale.h>
#include <iomanip>
#include <sstream>

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

std::string StringUtil::intToString(int number) {
    return std::to_string(number);
}

std::string StringUtil::floatToString(float number, int decimalDigits) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(decimalDigits) << number;
    return oss.str();
}

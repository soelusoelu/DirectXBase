#include "StringUtil.h"
#include <locale.h>
#include <iomanip>
#include <sstream>

std::vector<std::string> StringUtil::split(const std::string& src, const char delimiter) {
    StringArray temp;
    auto s = src;
    auto pos = s.find(delimiter);
    while (pos != std::string::npos) {
        auto sub = s.substr(0, pos);
        temp.emplace_back(sub);
        s = s.substr(pos + 1);
        pos = s.find(delimiter);
    }
    temp.emplace_back(s);

    return temp;
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

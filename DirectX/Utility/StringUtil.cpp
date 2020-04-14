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

std::string StringUtil::boolToString(bool value) {
    return (value) ? "true" : "false";
}

std::string StringUtil::intToString(int number) {
    return std::to_string(number);
}

std::string StringUtil::floatToString(float number, int decimalDigits) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(decimalDigits) << number;
    return oss.str();
}

std::string StringUtil::vector2ToString(const Vector2& vec, int decimalDigits) {
    auto x = "X " + floatToString(vec.x, decimalDigits);
    auto y = "Y " + floatToString(vec.y, decimalDigits);
    return x + "  " + y;
}

std::string StringUtil::vector3ToString(const Vector3& vec, int decimalDigits) {
    auto x = "X " + floatToString(vec.x, decimalDigits);
    auto y = "Y " + floatToString(vec.y, decimalDigits);
    auto z = "Z " + floatToString(vec.z, decimalDigits);
    return x + "  " + y + "  " + z;
}

std::string StringUtil::quaternionToString(const Quaternion& quaternion, int decimalDigits) {
    auto x = "X " + floatToString(quaternion.x, decimalDigits);
    auto y = "Y " + floatToString(quaternion.y, decimalDigits);
    auto z = "Z " + floatToString(quaternion.z, decimalDigits);
    auto w = "W " + floatToString(quaternion.w, decimalDigits);
    return x + "  " + y + "  " + z + "  " + w;
}

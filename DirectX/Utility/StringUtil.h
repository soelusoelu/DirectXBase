#pragma once

#include <string>

class StringUtil {
public:
    //デリミタが見つかるまでの文字列を返す
    static std::string spritFirst(const std::string& string, const char delimiter);
    static wchar_t* charToWchar(const char* src);
};

#pragma once

//何でも文字列に変換
#define TO_STRING(X) # X;

#include <string>

class StringUtil {
public:
    //デリミタが見つかるまでの文字列を返す
    static std::string spritFirst(const std::string& string, const char delimiter);
    //charからWcharへの文字変換
    static wchar_t* charToWchar(const char* src);
    //intからstringへの変換
    static std::string intToString(int number);
    //floatからstringへの変換
    static std::string floatToString(float number, int decimalDigits = 1);
};

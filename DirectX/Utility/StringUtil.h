#pragma once

//何でも文字列に変換
#define TO_STRING(X) # X;

#include <string>
#include <vector>

class StringUtil {
    using StringArray = std::vector<std::string>;

public:
    //デリミタごとに文字列作成
    static StringArray split(const std::string& src, const char delimiter);
    //charからWcharへの文字変換
    static wchar_t* charToWchar(const char* src);
    //intからstringへの変換
    static std::string intToString(int number);
    //floatからstringへの変換
    static std::string floatToString(float number, int decimalDigits = 1);
};

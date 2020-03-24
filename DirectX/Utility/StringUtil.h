﻿#pragma once

//何でも文字列に変換
#define TO_STRING(X) # X;

#include "Math.h"
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
    static std::string floatToString(float number, int decimalDigits = 2);
    //Vector2からstringへの変換
    static std::string vector2ToString(const Vector2& vec, int decimalDigits = 2);
    //Vector3からstringへの変換
    static std::string vector3ToString(const Vector3& vec, int decimalDigits = 2);
    //Quaternionからstringへの変換
    static std::string quaternionToString(const Quaternion& quaternion, int decimalDigits = 2);
};

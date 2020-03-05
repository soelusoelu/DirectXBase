#pragma once

#include <string>

class FileUtil {
public:
    static std::string getFileExtension(const std::string& filePath);
    static std::string getFileNameFromDirectry(const std::string& filePath);
    static std::string getDirectryFromFilePath(const std::string& filePath);
};

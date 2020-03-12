#pragma once

#include <string>

class Directory {
public:
    static void initDirectory();
    static void setRootDirectory();
    static void setAssetsDirectory();
    static void setShaderDirectory();
    static void setTextureDirectory();
    static void setSoundDirectory();
    static void setDataDirectory();
    static void setModelDirectory(const std::string& directry);

private:
    static void setCurrentDirectory(const std::string& path);

private:
    static std::string mRootPath;
};

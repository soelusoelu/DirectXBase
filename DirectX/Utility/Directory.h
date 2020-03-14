#pragma once

#include "Singleton.h"
#include <string>

class Directory {
    friend class Singleton<Directory>;

public:
    Directory();
    ~Directory();
    void initialize();
    void setRootDirectory();
    void setAssetsDirectory();
    void setShaderDirectory();
    void setTextureDirectory();
    void setSoundDirectory();
    void setDataDirectory();
    void setModelDirectory(const std::string& directry);

private:
    void setCurrentDirectory(const std::string& path);

private:
    std::string mRootPath;
};

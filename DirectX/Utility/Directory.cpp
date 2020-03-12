#include "Directory.h"
#include "FileUtil.h"
#include "../System/DirectXIncLib.h"

void Directory::initDirectory() {
    char dir[1024];
    GetCurrentDirectoryA(sizeof(dir), dir);
    mRootPath = dir;
}

void Directory::setRootDirectory() {
    setCurrentDirectory(mRootPath);
}

void Directory::setAssetsDirectory() {
    auto temp = mRootPath + "\\Assets";
    setCurrentDirectory(temp);
}

void Directory::setShaderDirectory() {
    auto temp = mRootPath + "\\Shader";
    setCurrentDirectory(temp);
}

void Directory::setTextureDirectory() {
    auto temp = mRootPath + "\\Assets/Texture";
    setCurrentDirectory(temp);
}

void Directory::setSoundDirectory() {
    auto temp = mRootPath + "\\Assets/Sound";
    setCurrentDirectory(temp);
}

void Directory::setDataDirectory() {
    auto temp = mRootPath + "\\Assets/Data";
    setCurrentDirectory(temp);
}

void Directory::setModelDirectory(const std::string& directry) {
    auto temp = mRootPath + "\\Assets/Model/";
    temp += FileUtil::getDirectryFromFilePath(directry);

    setCurrentDirectory(temp);
}

void Directory::setCurrentDirectory(const std::string& path) {
    SetCurrentDirectoryA(path.c_str());
}

std::string Directory::mRootPath = "";

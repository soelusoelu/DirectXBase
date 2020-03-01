#include "FileUtil.h"
#include <algorithm>

std::string FileUtil::getFileExtension(const std::string& filePath) {
    std::string fileExtension;
    size_t pos = filePath.find_last_of('.');
    if (pos != std::string::npos) {
        fileExtension = filePath.substr(pos, filePath.length());

        std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);
    }

    return fileExtension;
}

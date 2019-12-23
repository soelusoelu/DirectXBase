#include "CSVReader.h"
#include "../System/Game.h"
#include <cassert>
#include <fstream>
#include <sstream>

CSVReader::CSVReader(const char* fileName) :
    mCSV(0),
    mWidthCount(0),
    mHeightCount(0) {
    parse(fileName);
}

CSVReader::~CSVReader() = default;

std::vector<int> CSVReader::load(const char* fileName) {
    parse(fileName);

    return mCSV;
}

std::vector<int> CSVReader::getParseData() const {
    return mCSV;
}

int CSVReader::getWidth() {
    return mWidthCount;
}

int CSVReader::getHeight() {
    return mHeightCount;
}

void CSVReader::parse(const char* fileName) {
    //中身リセット
    mCSV.clear();

    setDataDirectory();

    //読み込み開始
    std::ifstream ifs(fileName, std::ios::in);
    assert(ifs);

    std::string line;
    bool first = true;
    while (!ifs.eof()) {
        std::getline(ifs, line);

        const char delimiter = ',';
        for (const auto& s : line) {
            if (s != delimiter) {
                mCSV.emplace_back(s - 48);
            }
        }

        if (first) {
            first = false;
            mWidthCount = mCSV.size();
        }
    }
    mHeightCount = mCSV.size() / mWidthCount;
}

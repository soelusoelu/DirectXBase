#pragma once

#include <vector>

class CSVReader {
public:
    CSVReader(const char* fileName);
    ~CSVReader();
    std::vector<int> load(const char* fileName);
    std::vector<int> getParseData() const;
    int getWidth();
    int getHeight();

private:
    void parse(const char* fileName);

private:
    std::vector<int> mCSV;
    int mWidthCount;
    int mHeightCount;
};

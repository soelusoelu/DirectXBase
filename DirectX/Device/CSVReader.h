#pragma once

#include <string>
#include <vector>

class CSVReader {
public:
    CSVReader();
    CSVReader(const char* fileName);
    ~CSVReader();
    std::vector<int> load(const char* fileName);
    std::vector<std::string> loadString(const char* fileName);
    std::vector<int> getParseData() const;
    std::vector<std::string> getParseStringData() const;
    int getWidth();
    int getHeight();

private:
    void parse(const char* fileName);
    void parseString(const char* fileName);

private:
    std::vector<int> mCSV;
    std::vector<std::string> mCSVString;
    int mWidthCount;
    int mHeightCount;
};

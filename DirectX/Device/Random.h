#pragma once

#include <random>

class Random {
public:
    static void init();
    //min <= value <= max
    static int randomRange(int min, int max);
    static float randomRange(float min, float max);

    Random() = delete;
    ~Random() = delete;

private:
    static std::mt19937 mt;
};


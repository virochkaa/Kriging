#pragma once
#include <iostream>
#include <random>

namespace Math
{
    template <typename T>
    T Min(const T &val1, const T &val2)
    {
        if (val1 > val2)
            return val2;
        return val1;
    }
    template <typename T>
    T Max(const T &val1, const T &val2)
    {
        if (val1 > val2)
            return val1;
        return val2;
    }

    /*Алгоритм mt19937 для генерации псевдослучайных чисел*/
    int randomRange(const int &lowBoundary, const int &highBoundary);
    double randomRange(const double &lowBoundary, const double &highBoundary);
}
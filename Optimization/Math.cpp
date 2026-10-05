#include "Math.h"

int Math::randomRange(const int &lowBoundary, const int &highBoundary)
{
    if (highBoundary < lowBoundary)
        throw std::runtime_error("нижняя граница больше верхней!");
    thread_local static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> distr(lowBoundary, highBoundary);
    return distr(gen);
}

double Math::randomRange(const double &lowBoundary, const double &highBoundary)
{
    if (highBoundary < lowBoundary)
        throw std::runtime_error("нижняя граница больше верхней!");
    thread_local static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<double> distr(lowBoundary, highBoundary);
    return distr(gen);
}
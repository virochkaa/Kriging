#pragma once

#include "Dataset.h"
#include <functional>
#include <vector>

class GausSolver
{
private:
    const Dataset& data;

    std::vector<
        std::function<double(double, double, double)>
    > trendFunctions;

    Matrix trendMatrix;

public:
    explicit GausSolver(const Dataset& _data)
        : data(_data)
    {
    }

    void SetConstantTrend()
    {
        trendFunctions.clear();

        trendFunctions.push_back(
            [](double x, double y, double z)
            {
                return 1.0;
            }
        );
    }

    void SetLinearTrend()
    {
        SetConstantTrend();

        trendFunctions.push_back(
            [](double x, double y, double z)
            {
                return x;
            }
        );

        if (data.GetDimension() >= 2)
        {
            trendFunctions.push_back(
                [](double x, double y, double z)
                {
                    return y;
                }
            );
        }

        if (data.GetDimension() == 3)
        {
            trendFunctions.push_back(
                [](double x, double y, double z)
                {
                    return z;
                }
            );
        }
    }

    void BuildTrendMatrix()
    {
        trendMatrix.clear();

        for (std::size_t pointIdx = 0;
             pointIdx < data.size();
             ++pointIdx)
        {
            double x = data.getX(pointIdx);
            double y = 0.0;
            double z = 0.0;

            if (data.GetDimension() >= 2)
                y = data.getY(pointIdx);

            if (data.GetDimension() == 3)
                z = data.getZ(pointIdx);

            Vector trendVector;

            for (const auto& trendFunction : trendFunctions)
            {
                trendVector.push_back(
                    trendFunction(x, y, z)
                );
            }

            trendMatrix.push_back(trendVector);
        }
    }

    const Matrix& GetTrendMatrix() const
    {
        return trendMatrix;
    }
};
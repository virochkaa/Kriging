#include "TextReader.h"

#include <fstream>
#include <iostream>

int main()
{
    int dimension = 2;
    int fieldCount = 1;

    std::ifstream input("data/couette_10000.txt");

    Dataset data =
        TextReader::Read(input, dimension, fieldCount);

    std::cout
        << "Dimension: "
        << data.GetDimension()
        << '\n';

    std::cout
        << "Points: "
        << data.size()
        << '\n';

    std::size_t pointIdx = 5050;

    std::cout
        << "x = "
        << data.getX(pointIdx)
        << '\n';

    if (dimension >= 2)
    {
        std::cout
            << "y = "
            << data.getY(pointIdx)
            << '\n';
    }

    if (dimension == 3)
    {
        std::cout
            << "z = "
            << data.getZ(pointIdx)
            << '\n';
    }

    std::cout
        << "signal = "
        << data.GetSignal(0, pointIdx)
        << '\n';

    return 0;
}
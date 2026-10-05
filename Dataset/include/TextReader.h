#pragma once

#include "Dataset.h"
#include <istream>
#include <sstream>
#include <string>
#include <utility>

class TextReader
{
public:
    static Dataset Read(std::istream& input,
                        int dimension,
                        int fieldCount = 1)
    {
        Vector x, y, z;
        Matrix signal(fieldCount);

        std::string line;

        while (std::getline(input, line))
        {
            std::istringstream row(line);

            double value;

            switch (dimension)
            {
                case 1:
                    row >> value;
                    x.push_back(value);
                    break;

                case 2:
                    row >> value;
                    x.push_back(value);

                    row >> value;
                    y.push_back(value);
                    break;

                case 3:
                    row >> value;
                    x.push_back(value);

                    row >> value;
                    y.push_back(value);

                    row >> value;
                    z.push_back(value);
                    break;
            }

            for (int fieldIdx = 0;
                 fieldIdx < fieldCount;
                 ++fieldIdx)
            {
                row >> value;
                signal[fieldIdx].push_back(value);
            }
        }

        return Dataset(
            dimension,
            std::move(x),
            std::move(y),
            std::move(z),
            std::move(signal)
        );
    }
    #testoleg
};
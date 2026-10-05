#pragma once

#include "Dataset.h"
#include <algorithm>
#include <istream>
#include <iterator>
#include <sstream>
#include <string>

class TextReader {
public:
    static Dataset read(std::istream& input, int dimension, bool skipHeader = false) {
        if (skipHeader) {
            std::string header;
            std::getline(input, header);
        }

        std::string text((std::istreambuf_iterator<char>(input)),
                          std::istreambuf_iterator<char>());
        std::replace(text.begin(), text.end(), ',', ' ');
        std::replace(text.begin(), text.end(), ';', ' ');
        std::istringstream numbers(text);
        Dataset data(dimension);

        double x1, x2, x3, response;
        switch (dimension) {
            case 1:
                while (numbers >> x1 >> response)
                    data.addPoint({x1}, response);
                break;
            case 2:
                while (numbers >> x1 >> x2 >> response)
                    data.addPoint({x1, x2}, response);
                break;
            case 3:
                while (numbers >> x1 >> x2 >> x3 >> response)
                    data.addPoint({x1, x2, x3}, response);
                break;
        }
        return data;
    }

    static Dataset read(const std::string& text, int dimension) {
        std::istringstream input(text);
        return read(input, dimension);
    }
};

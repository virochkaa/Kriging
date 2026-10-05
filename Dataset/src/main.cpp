#include "TextReader.h"
#include <fstream>
#include <iostream>

int main() {
    const int dimension = 2;
    std::ifstream input("data/couette_10000.txt");
    Dataset data = TextReader::read(input, dimension);

    // Full matrix and vector, each accessible separately without copying.
    const Matrix& X = data.getX();
    const Vector& y = data.getY();

    std::cout << "dimension = " << data.dimension() << '\n';
    std::cout << "points = " << data.size() << '\n';
    std::cout << "X[0] = (";
    for (double coordinate : data.getPoint(0))
        std::cout << coordinate << ' ';
    std::cout << "), y[0] = " << data.getResponse(0) << '\n';
    std::cout << "X[5050][1] = " << data.getCoordinate(5050, 1) << '\n';
    std::cout << "X rows = " << X.size() << ", y length = " << y.size() << '\n';
    return 0;
}

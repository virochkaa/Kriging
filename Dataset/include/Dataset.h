#pragma once

#include <cstddef>
#include <utility>
#include <vector>
#include <functional>

using Vector = std::vector<double>;
using Matrix = std::vector<Vector>;

class Dataset 
{
private:
    int dimension;
    Vector x,y,z;
    Matrix signal;

public:
    explicit Dataset(int _dimension) : dimension(_dimension) {}

    Dataset(int _dimension, Vector _X, Vector _Y, Vector _Z, Matrix _signal)
        : dimension(_dimension), x(std::move(_X)),y(std::move(_Y)),z(std::move(_Z)), signal(std::move(_signal)) {}


    int GetDimension() const { return dimension; }
    std::size_t size() const { return x.size(); }

    double getX(std::size_t serialPointIdx) const 
    { 
      return x[serialPointIdx];
    }
     double getY(std::size_t serialPointIdx) const 
    { 
      return y[serialPointIdx];
    }
     double getZ(std::size_t serialPointIdx) const 
    { 
      return z[serialPointIdx];
    }
    double GetSignal(std::size_t fieldIdx, std::size_t serialPointIdx) const { return signal[fieldIdx][serialPointIdx]; }

};
class GausSolver
{
    std::function<double(double,double,double)> trendFunction = [](double x, double y=0, double z =0) {return 1.;};
    Vector CorrelationMatrix;
    Vector DispersionVector;
    Vector TrendedSignalMatrix;
}

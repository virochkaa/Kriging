#pragma once
#include "Optimizator.h"
#include "DataBank.h"

template<class Optimizator>
class Interface
{
public:
    Interface() = default;
    ~Interface() = default;

    void SetOptimizator(const std::string &directory, const std::string &fileName) 
    {
        listener.DataCollection(directory, fileName, &data);
        data.CheckData();
    }

    void Optimize() 
    {
        optimizator.Solver(data, signal);
        data.DataMemoryClear();
    }
    void SetSignal(ExternalSignal& _signal) 
    {
        signal = _signal;
    }

private:

    //Коллекция оптимизаторов
    Optimizator optimizator;
    DataBank data;
    Listener listener;
    ExternalSignal signal;

};
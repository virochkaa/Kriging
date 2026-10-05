#include <iostream>
#include "Interface.h"

int main()
{
    const std::string folder = "data";
    const std::string file = "CADFlo.txt";
    
    ExternalSignal signal; signal.Set(Signals::CADFlo);
    Interface <NSGA2> interface;
    interface.SetOptimizator(folder, file);
    interface.SetSignal(signal);
    interface.Optimize();
}
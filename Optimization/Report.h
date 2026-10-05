#pragma once
#include <fstream>
#include <iostream>
#include "Populations.h"
#include "Math.h"

namespace Report
{
    /* Function to print the information of a population in a file */
    void reportPop(Population *pop, std::ofstream &file, DataBank &data, bool final = false);
}
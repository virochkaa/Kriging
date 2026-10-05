#pragma once

#include <cstdio>
#include "Memory.h"

class DataBank
{
public:
    DataBank() = default;
    ~DataBank() = default;

    // Режимы вычисления

    int nreal = 0;
    int nbin = 0;
    int nobj = 0;
    int ncon = 0;
    int popsize = 0;
    double pcross_real = 0;
    double pcross_bin = 0;
    double pmut_real = 0;
    double pmut_bin = 0;
    double eta_c = 0;
    double eta_m = 0;
    int ngen = 0;
    int nbinmut = 0;
    int nrealmut = 0;
    int nbincross = 0;
    int nrealcross = 0;
    size_t *nbits = nullptr;
    double *min_realvar = nullptr;
    double *max_realvar = nullptr;
    double *min_binvar = nullptr;
    double *max_binvar = nullptr;
    int bitlength = 0;

    void DataMemoryClear()
    {
        Memory::Deallocate(nbits);
        Memory::Deallocate(min_realvar);
        Memory::Deallocate(max_realvar);
        Memory::Deallocate(min_binvar);
        Memory::Deallocate(max_binvar);
    }

    void CheckData()
    {
        bool checkData = false;
        if (popsize % 4)
        {
            std::cerr << "The population size must be a multiple of 4 " << std::endl;
            checkData = true;
        }
        if (popsize <= 0)
        {
            std::cerr << "The population size must be a positive " << std::endl;
            checkData = true;
        }
        if (ngen < 1)
        {
            std::cerr << "The number of generations must be more than 1 " << std::endl;
            checkData = true;
        }
        if (nobj < 1)
        {
            std::cerr << "The number of objects must be more than 1 " << std::endl;
            checkData = true;
        }
        if (ncon < 0)
        {
            std::cerr << "The number of constraints must be non-negative " << std::endl;
            checkData = true;
        }
        if (nreal < 0)
        {
            std::cerr << "The number of real variable must be non-negative " << std::endl;
            checkData = true;
        }
        if (nreal > 0)
        {
            for (auto i = 0; i < nreal; ++i)
            {
                if (max_realvar[i] <= min_realvar[i])
                {
                    std::cerr << "The upper limit for a real variable must be greater than the lower limit" << std::endl;
                    checkData = true;
                    break;
                }
            }
        }
        if (pcross_real < 0.0 || pcross_real > 1.0)
        {
            std::cerr << "The crossover probability for real variable must be greater than 0 and less than 1" << std::endl;
            checkData = true;
        }
        if (pmut_real < 0.0 || pmut_real > 1.0)
        {
            std::cerr << "The crossover mutation for real variable must be greater than 0 and less than 1" << std::endl;
            checkData = true;
        }
        if (eta_c <= 0 && nreal)
        {
            std::cerr << "The value of distribution index for crossover must be positive" << std::endl;
            checkData = true;
        }
        if (eta_m <= 0 && nreal)
        {
            std::cerr << "The value of distribution index for mutation must be positive" << std::endl;
            checkData = true;
        }
        if (nbin < 0)
        {
            std::cerr << "The number of bin variable must be non-negative " << std::endl;
            checkData = true;
        }
        if (nbin > 0)
        {
            for (auto i = 0; i < nbin; ++i)
            {
                if (max_binvar[i] <= min_binvar[i])
                {
                    std::cerr << "The upper limit for a bin variable must be greater than the lower limit" << std::endl;
                    checkData = true;
                    break;
                }
            }
            for (auto i = 0; i < nbin; ++i)
            {
                if (nbits[i] < 1)
                {
                    std::cerr << "The number of bits for binary variable must be greater than 1" << std::endl;
                    checkData = true;
                    break;
                }
            }
        }

        if (pcross_bin < 0.0 || pcross_bin > 1.0)
        {
            std::cerr << "The crossover probability for binary variable must be greater than 0 and less than 1" << std::endl;
            checkData = true;
        }
        std::cout << pmut_bin << std::endl;
        std::cout << pcross_bin << std::endl;
        if (pmut_bin < 0.0 || pmut_bin > 1.0)
        {
            std::cerr << "The crossover mutation for binary variable must be greater than 0 and less than 1" << std::endl;
            checkData = true;
        }
        if (nreal == 0 && nbin == 0)
        {
            std::cerr << "Number of real as well as binary variables, both are zero, hence exiting" << std::endl;
            checkData = true;
        }
        if (checkData)
            std::exit(EXIT_FAILURE);
    }

private:
    /* data */
};

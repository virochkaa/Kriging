#pragma once
#include "Math.h"
#include "DataBank.h"
#include "Memory.h"

struct Individual
{
    void allocate(DataBank &data);

    void deallocate(DataBank &data);

    void randomInitialize(DataBank &data);

    /*Из бинарного представления числа получаем вещественное*/
    void decodeBinToReal(DataBank &data);

    /*Типо конструктор копирования*/
    void copyInd(Individual *other, DataBank &data);

    int rank = 0;
    double constr_violation = 0;
    double crowd_dist = 0;
    double *xreal = nullptr;
    double *xbin = nullptr;
    double *obj = nullptr;
    double *constr = nullptr;
    int **gene = nullptr;
};

struct Population
{
    void allocate(int _size, DataBank &data);

    void deallocate(DataBank &data);

    void randomInitialize(DataBank &data);

    void decodeBinToReal(DataBank &data);
    
    Individual *ind = nullptr;
    int size = 0;
};
#pragma once
#include "Populations.h"
#include "LinkedList.h"
#include <iostream>
#include <fstream>

namespace Signals
{
    void osy(double *xreal, double *xbin, int **gene, double *obj, double *constr);
    void zdt5(double *xreal, double *xbin, int **gene, double *obj, double *constr);
    void zdt6(double *xreal, double *xbin, int **gene, double *obj, double *constr);
    void CADFlo(double *xreal, double *xbin, int **gene, double *obj, double *constr);
}

class ExternalSignal
{
public:
    ExternalSignal() = default;

    void Set(void (*_ptrtofunc)(double *, double *, int **, double *, double *)) { ptrtofunc = _ptrtofunc; };

    void Operate(double *xreal, double *xbin, int **gene, double *obj, double *constr)
    {
        return ptrtofunc(xreal, xbin, gene, obj, constr);
    }

private:
    void (*ptrtofunc)(double *, double *, int **, double *, double *) = nullptr;
};

/*функции сортировки*/
namespace Sorting
{
    void evaluateInd(Individual *ind, int ncon, ExternalSignal &signal);

    /* Процедура вычисления значений целевой функции и подсчета НЕудовлетворенных ограничений для популяции */
    void evaluatePop(Population *pop, DataBank &data, ExternalSignal &signal);

    /* Routine for usual non-domination checking
    It will return the following values
    1 if a dominates b
    -1 if b dominates a
    0 if both a and b are non-dominated */
    int checkDominance(Individual *a, Individual *b, DataBank& data);

    /* Randomized quick sort routine to sort a population based on a particular objective chosen */
    void quickSortFrontObj(Population *pop, int *objArray, int objCount, int objArraySize);

    /* Randomized quick sort routine to sort a population based on crowding distance */
    void quickSortDist(Population *pop, int *dist, int frontSize);

    /* Actual implementation of the randomized quick sort used to sort a population based on crowding distance */
    void qSortDist(Population *pop, int *dist, int left, int right);

    /* Actual implementation of the randomized quick sort used to sort a population based on a particular objective chosen */
    void qSortFrontObj(Population *pop, int *objArray, int objCount, int left, int right);
}

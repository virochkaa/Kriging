#pragma once

# define INF 1.0e14
# define EPS 1.0e-14
# define E  2.71828182845905
# define PI 3.14159265358979

#include "Populations.h"
#include "Math.h"

/*Безымянные переменные нужно исправить*/
/*В этом namespace собраны функции, которые позволяют вносить изменения в индивидуумов, позволяя повысить разнообразие популяции*/
namespace Operators
{
    /*Функция копирует кусок данных первого массива во второй в то же место */
    template<typename T>
    void copySegment(const T* arr1, T* arr2, int startSegment, int endSegment)
    {
        for(int arrIndx = startSegment; arrIndx < endSegment; ++arrIndx)
            arr2[arrIndx] = arr1[arrIndx];
    }
    
    /*функция изменяет значение val в том случае, если val не лежит в границах
    val = Math::Max(lowVal, Math::Min(upVal, val))*/
    void clamp(double& val, const double& lowVal, const double& upVal);

    /*функция вычисляет параметр beta_q для SBX рекомбинации*/
    double computeBetaq(double rand, double eta_c, double koef);

    /*функция вычисляет параметр delta_q для мутации*/
    double computeDeltaq(double rand, double eta_m, double delta1, double delta2);

    /*Функция для SBX рекомбинации генов для вещественных параметров
    С некоторой вероятностью перемешивает параметры у детей, 
    чтобы увеличить многообразие потомства*/
    void realCross(Individual *parent1, Individual *parent2, Individual *child1, Individual *child2, DataBank &data);

    /*Функция для SBX рекомбинации генов для бинарных параметров
    С некоторой вероятностью перемешивает параметры у детей, 
    чтобы увеличить многообразие потомства*/
    void binCross(Individual *parent1, Individual *parent2, Individual *child1, Individual *child2, DataBank& data);

    /* Функция рекомбинации и вещественных и бинарных параметров */
    void crossover(Individual *parent1, Individual *parent2, Individual *child1, Individual *child2, DataBank &data);

    /* Routine for real polynomial mutation of an individual */
    void realMutateInd (Individual *ind, DataBank &data);

    void binMutateInd (Individual *ind, DataBank &data);

    /* Function to perform mutation of an individual */
    void mutationInd (Individual *ind, DataBank &data);

    /* Function to perform mutation in a population */
    void mutationPop (Population *pop, DataBank &data);

    /* Routine to merge two populations into one */
    void mergePop(Population *pop1, Population *pop2, Population *pop3, DataBank &data);
}
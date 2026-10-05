#pragma once

#define INF 1.0e14
#define EPS 1.0e-14
#define E 2.71828182845905
#define PI 3.14159265358979

#include "Operators.h"
#include "Populations.h"
#include "Sorting.h"
#include "DataBank.h"

namespace Diversity
{
    /* Routine to compute crowding distances */
    /*Я вообще не понимаю, что здесь происходит*/
    void assignCrowdingDistance(Population *pop, int **obj_array, int *dist, int frontSize, DataBank &data);
    
    /* Routine to compute crowding distance based on objective function values when the population in in the form of an array */
    void assignCrowdingDistanceIndices(Population *pop, int frontStart, int frontEnd, DataBank &data);

    /* Routine to compute crowding distance based on ojbective function values when the population in in the form of a list */
    void assignCrowdingDistanceList(Population *pop, list *lst, int frontSize, DataBank &data);

    /*Эта функция проходится по всем элементам из pool и если по ним строит elite. Если элемент в elite, то его нет в pool*/
    void CurrentFrontPareto(Population *mixedPop, list *pool, list *elite, int &frontSize, DataBank &data);

    /* Routine to fill a population with individuals in the decreasing order of crowding distance */
    void crowdingFill(Population *mixedPop, Population *newPop, list *elite, int count, int frontSize, DataBank &data);

    /*После того как веделили фронт Парето, заполняем новую популяцию выгодными элементами и сортируем их*/
    void FillCurrentParetoToNewPop(Population *mixedPop, Population *newPop, list *elite, int &archieveSize, const int frontSize,
                                    int &end, int &rank, DataBank &data);

    /* Routine to perform non-dominated sorting */
    void fill_nondominated_sort(Population *mixedPop, Population *newPop, DataBank &data);

    /* Function to assign rank and crowding distance to a population of size pop_size*/
    void assign_rank_and_crowding_distance(Population *newPop, DataBank &data);

    /* Routine for binary tournament */
    Individual *tournament(Individual *ind1, Individual *ind2, DataBank &data);

    /* Routine for tournament selection, it creates a newPop from oldPop by performing tournament selection and the crossover */
    void selection(Population *oldPop, Population *newPop, DataBank &data);
}
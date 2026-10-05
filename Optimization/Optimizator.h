#pragma once

#include "Populations.h"
#include "DataBank.h"
#include "Sorting.h"
#include "Diversity.h"
#include "Report.h"
#include "Listener.h"

class NSGA2
{

private:
    Population *parent_pop = nullptr;
    Population *child_pop = nullptr;
    Population *mixed_pop = nullptr;

    // void assign_rank_and_crowding_distance (Population *newPop, int nobj);

public:
    NSGA2() = default;
    ~NSGA2() = default;

    void Solver(DataBank &data, ExternalSignal &signal)
    {
        std::ofstream fpt3("best_pop.out");
        std::ofstream fpt4("all_pop.out");
        Memory::AllocateObj(parent_pop);
        Memory::AllocateObj(child_pop);
        Memory::AllocateObj(mixed_pop);
        parent_pop->allocate(data.popsize, data);
        child_pop->allocate(data.popsize, data);
        mixed_pop->allocate(2 * data.popsize, data);
        parent_pop->randomInitialize(data);
        parent_pop->decodeBinToReal(data);
        Sorting::evaluatePop(parent_pop, data, signal);
        Diversity::assign_rank_and_crowding_distance(parent_pop, data);
        fpt4 << "# gen = 1\n";
        Report::reportPop(parent_pop, fpt4, data);

        for (int i = 2; i <= data.ngen; i++)
        {
            Diversity::selection(parent_pop, child_pop, data);
            Operators::mutationPop(child_pop, data);
            child_pop->decodeBinToReal(data);
            Sorting::evaluatePop(child_pop, data, signal);
            Operators::mergePop(parent_pop, child_pop, mixed_pop, data);
            Diversity::fill_nondominated_sort(mixed_pop, parent_pop, data);
            fpt4 << "# gen = " << i << '\n';
            Report::reportPop(parent_pop,  fpt4,  data);
        }
        Report::reportPop(parent_pop, fpt3, data, true);
    }
};

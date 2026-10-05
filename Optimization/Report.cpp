#include "Report.h"

void Report::reportPop(Population *pop, std::ofstream &file, DataBank &data, bool final)
{
    for (int indIndx = 0; indIndx < data.popsize; ++indIndx)
    {
        if (final && !(pop->ind[indIndx].constr_violation == 0.0 && pop->ind[indIndx].rank == 1))
            continue;
        for (int objIndx = 0; objIndx < data.nobj; ++objIndx)
            file << std::scientific << pop->ind[indIndx].obj[objIndx] << "\t";

        for (int conIndx = 0; conIndx < data.ncon; ++conIndx)
            file << std::scientific << pop->ind[indIndx].constr[conIndx] << "\t";

        for (int xrealIndx = 0; xrealIndx < data.nreal; ++xrealIndx)
            file << std::scientific << pop->ind[indIndx].xreal[xrealIndx] << "\t";

        for (int binIndx = 0; binIndx < data.nbin; ++binIndx)
        {
            for (size_t bitBinIndx = 0; bitBinIndx < data.nbits[binIndx]; ++bitBinIndx)
            {
                file << pop->ind[indIndx].gene[binIndx][bitBinIndx] << "\t";
            }
        }
        file << std::scientific << pop->ind[indIndx].constr_violation << "\t";
        file << pop->ind[indIndx].rank << "\t";
        file << std::scientific << pop->ind[indIndx].crowd_dist << "\n";
    }
}
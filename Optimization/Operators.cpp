#include "Operators.h"

void Operators::clamp(double &val, const double &lowVal, const double &upVal)
{
    if (val < lowVal)
        val = lowVal;
    else if (val > upVal)
        val = upVal;
}

double Operators::computeBetaq(double rand, double eta_c, double koef)
{
    double beta = 1 + 2 * koef;
    double alpha = 2.0 - pow(beta, -(eta_c + 1.0));
    if (rand > (1.0 / alpha))
        return pow((1.0 / (2.0 - rand * alpha)), (1.0 / (eta_c + 1.0)));
    else
        return pow((rand * alpha), (1.0 / (eta_c + 1.0)));
}

double Operators::computeDeltaq(double rand, double eta_m, double delta1, double delta2)
{
    double mut_pow = 1.0 / (eta_m + 1.0);
    double xy = 0;
    double val = 0;
    if (rand <= 0.5)
    {
        xy = 1.0 - delta1;
        val = 2.0 * rand + (1.0 - 2.0 * rand) * (pow(xy, (eta_m + 1.0)));
        return pow(val, mut_pow) - 1.0;
    }
    else
    {
        xy = 1.0 - delta2;
        val = 2.0 * (1.0 - rand) + 2.0 * (rand - 0.5) * (pow(xy, (eta_m + 1.0)));
        return 1.0 - (pow(val, mut_pow));
    }
}

void Operators::realCross(Individual *parent1, Individual *parent2, Individual *child1, Individual *child2, DataBank &data)
{
    double rand;
    double y1, y2, yl, yu;
    double c1, c2;
    double betaq;
    if (Math::randomRange(0.0, 1.0) > data.pcross_real)
    {
        copySegment(parent1->xreal, child1->xreal, 0, data.nreal);
        copySegment(parent2->xreal, child2->xreal, 0, data.nreal);
        return;
    }

    data.nrealcross++;
    for (int xrealIndx = 0; xrealIndx < data.nreal; ++xrealIndx)
    {
        if (Math::randomRange(0.0, 1.0) > 0.5 || fabs(parent1->xreal[xrealIndx] - parent2->xreal[xrealIndx]) < EPS)
        {
            child1->xreal[xrealIndx] = parent1->xreal[xrealIndx];
            child2->xreal[xrealIndx] = parent2->xreal[xrealIndx];
            continue;
        }

        y1 = Math::Min(parent1->xreal[xrealIndx], parent2->xreal[xrealIndx]);
        y2 = Math::Max(parent1->xreal[xrealIndx], parent2->xreal[xrealIndx]);

        yl = data.min_realvar[xrealIndx];
        yu = data.max_realvar[xrealIndx];

        rand = Math::randomRange(0.0, 1.0);
        betaq = computeBetaq(rand, data.eta_c, (y1 - yl) / (y2 - y1));
        c1 = 0.5 * ((y1 + y2) - betaq * (y2 - y1));
        betaq = computeBetaq(rand, data.eta_c, (yu - y2) / (y2 - y1));
        c2 = 0.5 * ((y1 + y2) + betaq * (y2 - y1));

        clamp(c1, yl, yu);
        clamp(c2, yl, yu);

        if (Math::randomRange(0.0, 1.0) <= 0.5)
        {
            child1->xreal[xrealIndx] = c2;
            child2->xreal[xrealIndx] = c1;
        }
        else
        {
            child1->xreal[xrealIndx] = c1;
            child2->xreal[xrealIndx] = c2;
        }
    }
}

void Operators::binCross(Individual *parent1, Individual *parent2, Individual *child1, Individual *child2, DataBank &data)
{
    int startSegment = 0;
    int endSegment = 0;
    for (int binIndx = 0; binIndx < data.nbin; ++binIndx)
    {
        if (Math::randomRange(0.0, 1.0) > data.pcross_bin)
        {
            copySegment(parent1->gene[binIndx], child1->gene[binIndx], 0, data.nbits[binIndx]);
            copySegment(parent2->gene[binIndx], child2->gene[binIndx], 0, data.nbits[binIndx]);
            continue;
        }
        data.nbincross++;
        startSegment = Math::randomRange(0, data.nbits[binIndx] - 1);
        endSegment = Math::randomRange(0, data.nbits[binIndx] - 1);

        if (startSegment > endSegment)
            std::swap(startSegment, endSegment);

        copySegment(parent1->gene[binIndx], child1->gene[binIndx], 0, startSegment);
        copySegment(parent2->gene[binIndx], child1->gene[binIndx], startSegment, endSegment);
        copySegment(parent1->gene[binIndx], child1->gene[binIndx], endSegment, data.nbits[binIndx]);

        copySegment(parent2->gene[binIndx], child2->gene[binIndx], 0, startSegment);
        copySegment(parent1->gene[binIndx], child2->gene[binIndx], startSegment, endSegment);
        copySegment(parent2->gene[binIndx], child2->gene[binIndx], endSegment, data.nbits[binIndx]);
    }
}

void Operators::crossover(Individual *parent1, Individual *parent2, Individual *child1, Individual *child2, DataBank &data)
{
    if (data.nreal != 0)
        realCross(parent1, parent2, child1, child2, data);
    if (data.nbin != 0)
        binCross(parent1, parent2, child1, child2, data);
}

void Operators::realMutateInd(Individual *ind, DataBank &data)
{
    double delta1, delta2, deltaq;
    double y, yl, yu;
    for (int xrealIndx = 0; xrealIndx < data.nreal; ++xrealIndx)
    {
        if (Math::randomRange(0.0, 1.0) > data.pmut_real)
            continue;
        data.nrealmut += 1;

        y = ind->xreal[xrealIndx];
        yl = data.min_realvar[xrealIndx];
        yu = data.max_realvar[xrealIndx];

        delta1 = (y - yl) / (yu - yl);
        delta2 = (yu - y) / (yu - yl);
        deltaq = computeDeltaq(Math::randomRange(0.0, 1.0), data.eta_m, delta1, delta2);

        y = y + deltaq * (yu - yl);
        clamp(y, yl, yu);
        ind->xreal[xrealIndx] = y;
    }
}

void Operators::binMutateInd(Individual *ind, DataBank &data)
{
    for (int binIndx = 0; binIndx < data.nbin; ++binIndx)
    {
        for (size_t bitBinIndx = 0; bitBinIndx < data.nbits[binIndx]; ++bitBinIndx)
        {
            if (Math::randomRange(0.0, 1.0) > data.pmut_bin)
                continue;
            data.nbinmut += 1;

            if (ind->gene[binIndx][bitBinIndx] == 0)
                ind->gene[binIndx][bitBinIndx] = 1;
            else
                ind->gene[binIndx][bitBinIndx] = 0;
        }
    }
}

void Operators::mutationInd(Individual *ind, DataBank &data)
{
    if (data.nreal != 0)
        realMutateInd(ind, data);
    if (data.nbin != 0)
        binMutateInd(ind, data);
}

void Operators::mutationPop(Population *pop, DataBank &data)
{
    for (int indIndx = 0; indIndx < data.popsize; ++indIndx)
        mutationInd(&(pop->ind[indIndx]), data);
}

void Operators::mergePop(Population *pop1, Population *pop2, Population *pop3, DataBank &data)
{
    for (int indIndx = 0; indIndx < data.popsize; ++indIndx)
        (pop3->ind[indIndx]).copyInd(&(pop1->ind[indIndx]), data);
    for (int indIndx = 0; indIndx < data.popsize; ++indIndx)
        (pop3->ind[indIndx + data.popsize]).copyInd(&(pop2->ind[indIndx]), data);
}

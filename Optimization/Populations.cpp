#include "Populations.h"

void Individual::allocate(DataBank &data)
{
    Memory::Allocate(xreal, data.nreal);
    Memory::Allocate(xbin, data.nbin);
    Memory::Allocate(gene, data.nbin, data.nbits);
    Memory::Allocate(obj, data.nobj);
    Memory::Allocate(constr, data.ncon);
}

void Individual::deallocate(DataBank &data)
{
    Memory::Deallocate(xreal);
    Memory::Deallocate(xbin);
    Memory::Deallocate(gene, data.nbin);
    Memory::Deallocate(obj);
    Memory::Deallocate(constr);
}

void Individual::randomInitialize(DataBank &data)
{
    for (int realVarIndx = 0; realVarIndx < data.nreal; ++realVarIndx)
        xreal[realVarIndx] = Math::randomRange(data.min_realvar[realVarIndx], data.max_realvar[realVarIndx]); // namespace Math in Math.h

    for (int binVarIndx = 0; binVarIndx < data.nbin; ++binVarIndx)
    {
        for (size_t bitBinVarIndx = 0; bitBinVarIndx < data.nbits[binVarIndx]; ++bitBinVarIndx)
        {
            if (Math::randomRange(0.0, 1.0) <= 0.5)
                gene[binVarIndx][bitBinVarIndx] = 0;
            else
                gene[binVarIndx][bitBinVarIndx] = 1;
        }
    }
}

void Individual::decodeBinToReal(DataBank &data)
{
    int sum = 0;
    for (int binVarIndx = 0; binVarIndx < data.nbin; ++binVarIndx)
    {
        sum = 0;
        for (size_t bitBinVarIndx = 0; bitBinVarIndx < data.nbits[binVarIndx]; ++bitBinVarIndx)
            if (gene[binVarIndx][bitBinVarIndx])
                sum += pow(2, data.nbits[binVarIndx] - 1 - bitBinVarIndx);

        xbin[binVarIndx] = data.min_binvar[binVarIndx] + (double)sum * (data.max_binvar[binVarIndx] - data.min_binvar[binVarIndx]) / (double)(pow(2, data.nbits[binVarIndx]) - 1);
    }
}

void Individual::copyInd(Individual *other, DataBank &data)
{
    if (!other)
        return;
    this->rank = other->rank;
    this->constr_violation = other->constr_violation;
    this->crowd_dist = other->crowd_dist;
    std::copy(other->xreal, other->xreal + data.nreal, this->xreal);
    std::copy(other->xbin, other->xbin + data.nbin, this->xbin);
    std::copy(other->obj, other->obj + data.nobj, this->obj);
    std::copy(other->constr, other->constr + data.ncon, this->constr);
    for (int binIndx = 0; binIndx < data.nbin; ++binIndx)
    {
        std::copy(other->gene[binIndx], other->gene[binIndx] + data.nbits[binIndx], this->gene[binIndx]);
    }
}

void Population::allocate(int _size, DataBank &data)
{
    size = _size;
    Memory::Allocate(ind, size);
    for (int indIndx = 0; indIndx < size; ++indIndx)
    {
        ind[indIndx].allocate(data);
    }
}

void Population::deallocate(DataBank &data)
{
    for (int indIndx = 0; indIndx < size; indIndx++)
        ind[indIndx].deallocate(data);

    Memory::Deallocate(ind);
}

void Population::randomInitialize(DataBank &data)
{
    for (int indIndx = 0; indIndx < size; ++indIndx)
        ind[indIndx].randomInitialize(data);
}

void Population::decodeBinToReal(DataBank &data)
{
    for (int indIndx = 0; indIndx < size; ++indIndx)
        ind[indIndx].decodeBinToReal(data);
}
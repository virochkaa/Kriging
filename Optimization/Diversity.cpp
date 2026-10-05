#include "Diversity.h"

void Diversity::assignCrowdingDistance(Population *pop, int **obj_array, int *dist, int frontSize, DataBank &data)
{
    for (int objIndx = 0; objIndx < data.nobj; ++objIndx)
    {
        for (int frontIndx = 0; frontIndx < frontSize; ++frontIndx)
            obj_array[objIndx][frontIndx] = dist[frontIndx];

        Sorting::quickSortFrontObj(pop, obj_array[objIndx], objIndx, frontSize);
    }

    for (int frontIndx = 0; frontIndx < frontSize; ++frontIndx)
        pop->ind[dist[frontIndx]].crowd_dist = 0.0;

    for (int objIndx = 0; objIndx < data.nobj; ++objIndx)
        pop->ind[obj_array[objIndx][0]].crowd_dist = INF;

    for (int objIndx = 0; objIndx < data.nobj; ++objIndx)
    {
        for (int frontIndx = 1; frontIndx < frontSize - 1; ++frontIndx)
        {
            if (pop->ind[obj_array[objIndx][frontIndx]].crowd_dist == INF)
                continue;

            if (pop->ind[obj_array[objIndx][frontSize - 1]].obj[objIndx] == pop->ind[obj_array[objIndx][0]].obj[objIndx])
                pop->ind[obj_array[objIndx][frontIndx]].crowd_dist += 0.0;
            else
            {
                pop->ind[obj_array[objIndx][frontIndx]].crowd_dist +=
                    (pop->ind[obj_array[objIndx][frontIndx + 1]].obj[objIndx] - pop->ind[obj_array[objIndx][frontIndx - 1]].obj[objIndx]) /
                    (pop->ind[obj_array[objIndx][frontSize - 1]].obj[objIndx] - pop->ind[obj_array[objIndx][0]].obj[objIndx]);
            }
        }
    }
    for (int frontIndx = 0; frontIndx < frontSize; ++frontIndx)
    {
        if (pop->ind[dist[frontIndx]].crowd_dist == INF)
            continue;
        pop->ind[dist[frontIndx]].crowd_dist = (pop->ind[dist[frontIndx]].crowd_dist) / data.nobj;
    }
}

void Diversity::assignCrowdingDistanceIndices(Population *pop, int frontStart, int frontEnd, DataBank &data)
{
    int **obj_array = nullptr;
    int *dist = nullptr;
    int front_size = frontEnd - frontStart + 1;

    if (front_size == 1)
    {
        pop->ind[frontStart].crowd_dist = INF;
        return;
    }
    if (front_size == 2)
    {
        pop->ind[frontStart].crowd_dist = INF;
        pop->ind[frontEnd].crowd_dist = INF;
        return;
    }
    Memory::Allocate(obj_array, data.nobj, front_size);
    Memory::Allocate(dist, front_size);

    for (int frontIndx = 0; frontIndx < front_size; ++frontIndx)
        dist[frontIndx] = frontStart++;

    assignCrowdingDistance(pop, obj_array, dist, front_size, data);
    Memory::Deallocate(dist);
    Memory::Deallocate(obj_array, data.nobj);
}

void Diversity::assignCrowdingDistanceList(Population *pop, list *lst, int frontSize, DataBank &data)
{
    int **obj_array = nullptr;
    int *dist = nullptr;
    list *temp = lst;
    if (frontSize == 1)
    {
        pop->ind[lst->index].crowd_dist = INF;
        return;
    }
    if (frontSize == 2)
    {
        pop->ind[lst->index].crowd_dist = INF;
        pop->ind[lst->child->index].crowd_dist = INF;
        return;
    }

    Memory::Allocate(obj_array, data.nobj, frontSize);
    Memory::Allocate(dist, frontSize);

    for (int frontIndx = 0; frontIndx < frontSize; ++frontIndx)
    {
        dist[frontIndx] = temp->index;
        temp = temp->child;
    }
    assignCrowdingDistance(pop, obj_array, dist, frontSize, data);
    Memory::Deallocate(dist);
    Memory::Deallocate(obj_array, data.nobj);
}

void Diversity::crowdingFill(Population *mixedPop, Population *newPop, list *elite, int count, int frontSize, DataBank &data)
{
    int *dist = nullptr;
    list *tempElite = elite->child;
    Memory::Allocate(dist, frontSize);

    assignCrowdingDistanceList(mixedPop, elite->child, frontSize, data);
    for (int frontIndx = 0; frontIndx < frontSize; ++frontIndx)
    {
        dist[frontIndx] = tempElite->index;
        tempElite = tempElite->child;
    }
    Sorting::quickSortDist(mixedPop, dist, frontSize);
    for (int indIndx = count, frontIndx = frontSize - 1; indIndx < data.popsize; ++indIndx, frontIndx--)
        newPop->ind[indIndx].copyInd(&mixedPop->ind[dist[frontIndx]], data);

    Memory::Deallocate(dist);
}

void Diversity::CurrentFrontPareto(Population *mixedPop, list *pool, list *elite, int &frontSize, DataBank &data)
{
    list *tempPool = pool->child;
    list *tempElite = nullptr;
    int flag = 0;
    while (tempPool)
    {
        tempElite = elite->child;
        while (tempElite)
        {
            flag = Sorting::checkDominance(&(mixedPop->ind[tempPool->index]), &(mixedPop->ind[tempElite->index]), data);
            if (flag == 1)
            {
                insertNode(pool, tempElite->index);
                tempElite = deleteNode(tempElite);
                frontSize--;
                tempElite = tempElite->child;
            }
            if (flag == 0)
                tempElite = tempElite->child;
            if (flag == -1)
                break;
        }
        if (flag == 0 || flag == 1) // кандидат прошёл все проверки
        {
            insertNode(elite, tempPool->index); // добавляем в фронт
            frontSize++;                        // увеличиваем размер фронта
            tempPool = deleteNode(tempPool);    // убираем из pool
        }
        tempPool = tempPool->child; // следующий кандидат
    }
}

void Diversity::FillCurrentParetoToNewPop(Population *mixedPop, Population *newPop, list *elite, int &archieveSize, const int frontSize,
                                          int &end, int &rank, DataBank &data)
{
    list *tempElite = elite->child;
    int start = end;
    if ((archieveSize + frontSize) <= data.popsize)
    {
        while (tempElite)
        {
            newPop->ind[end].copyInd(&mixedPop->ind[tempElite->index], data);
            newPop->ind[end].rank = rank;
            archieveSize += 1;
            tempElite = tempElite->child;
            end += 1;
        }
        assignCrowdingDistanceIndices(newPop, start, end - 1, data);
        rank += 1;
    }
    else
    {
        crowdingFill(mixedPop, newPop, elite, end, frontSize, data);
        archieveSize = data.popsize;
        for (int popIndx = end; popIndx < data.popsize; ++popIndx)
            newPop->ind[popIndx].rank = rank;
    }
}

void Diversity::fill_nondominated_sort(Population *mixedPop, Population *newPop, DataBank &data)
{
    int end = 0;
    int frontSize = 0;
    int archieveSize = 0;
    int rank = 1;
    list *pool = nullptr;
    list *elite = nullptr;
    list *tempPool = nullptr;
    list *tempElite = nullptr;
    initList(pool);
    initList(elite);
    tempPool = pool;

    for (int mergePopIndx = 0; mergePopIndx < 2 * data.popsize; ++mergePopIndx)
    {
        insertNode(tempPool, mergePopIndx);
        tempPool = tempPool->child;
    }

    while (archieveSize < data.popsize)
    {
        frontSize = 1;
        tempPool = pool->child;
        insertNode(elite, tempPool->index);
        tempPool = deleteNode(tempPool);

        CurrentFrontPareto(mixedPop, pool, elite, frontSize, data);
        FillCurrentParetoToNewPop(mixedPop, newPop, elite, archieveSize, frontSize, end, rank, data);

        tempElite = elite;
        while (elite->child)
        {
            tempElite = tempElite->child;
            tempElite = deleteNode(tempElite);
        }
    }
    DeleteList(pool);
    DeleteList(elite);
}

void Diversity::assign_rank_and_crowding_distance(Population *newPop, DataBank &data)
{
    int frontSize = 0;
    int rank = 1;
    list *orig = nullptr;
    list *cur = nullptr;
    list *tempOrig = nullptr;
    list *tempCur = nullptr;

    initList(orig);
    initList(cur);

    tempOrig = orig;
    for (int indIndx = 0; indIndx < data.popsize; ++indIndx)
    {
        insertNode(tempOrig, indIndx);
        tempOrig = tempOrig->child;
    }
    while (orig->child)
    {
        if (!orig->child->child)
        {
            newPop->ind[orig->child->index].rank = rank;
            newPop->ind[orig->child->index].crowd_dist = INF;
            break;
        }
        frontSize = 1;
        tempOrig = orig->child;
        insertNode(cur, tempOrig->index);
        tempOrig = deleteNode(tempOrig);
        CurrentFrontPareto(newPop, orig, cur, frontSize, data);

        tempCur = cur->child;
        while (tempCur)
        {
            newPop->ind[tempCur->index].rank = rank;
            tempCur = tempCur->child;
        }
        assignCrowdingDistanceList(newPop, cur->child, frontSize, data);
        tempCur = cur;
        while (cur->child)
        {
            tempCur = tempCur->child;
            tempCur = deleteNode(tempCur);
        }
        rank += 1;
    }

    DeleteList(orig);
    DeleteList(cur);
}

Individual *Diversity::tournament(Individual *ind1, Individual *ind2, DataBank &data)
{
    int flag = 0;
    flag = Sorting::checkDominance(ind1, ind2, data);
    if (flag == 1)
        return (ind1);
    if (flag == -1)
        return (ind2);
    if (ind1->crowd_dist > ind2->crowd_dist)
        return (ind1);
    if (ind2->crowd_dist > ind1->crowd_dist)
        return (ind2);
    if (Math::randomRange(0.0, 1.0) <= 0.5)
        return (ind1);
    return (ind2);
}

void Diversity::selection(Population *oldPop, Population *newPop, DataBank &data)
{
    int *perm1 = nullptr;
    int *perm2 = nullptr;
    int rand = 0;
    Individual *parent1 = nullptr;
    Individual *parent2 = nullptr;
    Memory::Allocate(perm1, data.popsize);
    Memory::Allocate(perm2, data.popsize);

    for (int arrIndx = 0; arrIndx < data.popsize; ++arrIndx)
        perm1[arrIndx] = perm2[arrIndx] = arrIndx;

    for (int arrIndx = 0; arrIndx < data.popsize; ++arrIndx)
    {
        rand = Math::randomRange(arrIndx, data.popsize - 1);
        std::swap(perm1[rand], perm1[arrIndx]);
        rand = Math::randomRange(arrIndx, data.popsize - 1);
        std::swap(perm2[rand], perm2[arrIndx]);
    }

    for (int arrIndx = 0; arrIndx < data.popsize; arrIndx += 4)
    {
        if (arrIndx == 60)
        {
            int stop = 1;
            stop++;
        }

        parent1 = tournament(&oldPop->ind[perm1[arrIndx]], &oldPop->ind[perm1[arrIndx + 1]], data);
        parent2 = tournament(&oldPop->ind[perm1[arrIndx + 2]], &oldPop->ind[perm1[arrIndx + 3]], data);
        Operators::crossover(parent1, parent2, &newPop->ind[arrIndx], &newPop->ind[arrIndx + 1], data);
        parent1 = tournament(&oldPop->ind[perm2[arrIndx]], &oldPop->ind[perm2[arrIndx + 1]], data);
        parent2 = tournament(&oldPop->ind[perm2[arrIndx + 2]], &oldPop->ind[perm2[arrIndx + 3]], data);
        Operators::crossover(parent1, parent2, &newPop->ind[arrIndx + 2], &newPop->ind[arrIndx + 3], data);
    }
    Memory::Deallocate(perm1);
    Memory::Deallocate(perm2);
}
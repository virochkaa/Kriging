// #include "Optimizator.h"

// namespace Private
// {    
//     /* Routine to compute crowding distances */
//     /*Я вообще не понимаю, что здесь происходит*/
//     void assignCrowdingDistance (Population *pop, int **obj_array, int *dist, int frontSize, int nobj)
//     {
//         for (int objIndx = 0; objIndx < nobj; ++objIndx)
//         {
//             for (int frontIndx = 0; frontIndx < frontSize; ++frontIndx)
//                 obj_array[objIndx][frontIndx] = dist[frontIndx];
            
//             Sorting::quickSortFrontObj (pop, obj_array[objIndx], objIndx, frontSize);
//         }

//         for (int frontIndx = 0; frontIndx < frontSize; ++frontIndx)
//             pop->ind[dist[frontIndx]].crowd_dist = 0.0;

//         for (int objIndx = 0; objIndx < nobj; ++objIndx)
//             pop->ind[obj_array[objIndx][0]].crowd_dist = INF;

//         for (int objIndx = 0; objIndx < nobj; ++objIndx)
//         {
//             for (int frontIndx = 1; frontIndx < frontSize - 1; ++frontIndx)
//             {
//                 if (pop->ind[obj_array[objIndx][frontIndx]].crowd_dist == INF) continue;
                
//                 if (pop->ind[obj_array[objIndx][frontSize-1]].obj[objIndx] == pop->ind[obj_array[objIndx][0]].obj[objIndx]) pop->ind[obj_array[objIndx][frontIndx]].crowd_dist += 0.0;
//                 else
//                 {
//                     pop->ind[obj_array[objIndx][frontIndx]].crowd_dist += 
//                         (pop->ind[obj_array[objIndx][frontIndx+1]].obj[objIndx] - pop->ind[obj_array[objIndx][frontIndx-1]].obj[objIndx])/
//                         (pop->ind[obj_array[objIndx][frontSize-1]].obj[objIndx] - pop->ind[obj_array[objIndx][0]].obj[objIndx]);
//                 }
//             }
//         }
//         for (int frontIndx = 0; frontIndx < frontSize; ++frontIndx)
//         {
//             if (pop->ind[dist[frontIndx]].crowd_dist == INF) continue;
//             pop->ind[dist[frontIndx]].crowd_dist = (pop->ind[dist[frontIndx]].crowd_dist)/nobj;
//         }
//     }
//     void CurrentFrontPareto(Population *mixedPop, list *pool, list *elite, int &frontSize, int nobj)
//     {
//         list *tempPool = pool->child;
//         list *tempElite = nullptr;
//         int flag = 0;
//         while (tempPool)
//         {
//             tempElite = elite->child;
//             while (tempElite)
//             {
//                 flag = Sorting::checkDominance(&(mixedPop->ind[tempPool->index]), &(mixedPop->ind[tempElite->index]), nobj);
//                 if (flag == 1)
//                 {
//                     insertNode(pool, tempElite->index);
//                     tempElite = deleteNode(tempElite);
//                     frontSize--;
//                     tempElite = tempElite->child;
//                 }
//                 if (flag == 0)
//                     tempElite = tempElite->child;
//                 if (flag == -1)
//                     break;
//             }
//             if (flag == 0 || flag == 1) // кандидат прошёл все проверки
//             {
//                 insertNode(elite, tempPool->index); // добавляем в фронт
//                 frontSize++;                        // увеличиваем размер фронта
//                 tempPool = deleteNode(tempPool);    // убираем из pool
//             }
//             tempPool = tempPool->child; // следующий кандидат
//         }
//     }
//     /* Routine to compute crowding distance based on ojbective function values when the population in in the form of a list */
//     void assignCrowdingDistanceList(Population *pop, list *lst, int frontSize, int nobj)
//     {
//         int **obj_array = nullptr;
//         int *dist = nullptr;
//         list *temp = lst;
//         if (frontSize == 1)
//         {
//             pop->ind[lst->index].crowd_dist = INF;
//             return;
//         }
//         if (frontSize == 2)
//         {
//             pop->ind[lst->index].crowd_dist = INF;
//             pop->ind[lst->child->index].crowd_dist = INF;
//             return;
//         }

//         Memory::Allocate(obj_array, nobj, frontSize);
//         Memory::Allocate(dist, frontSize);

//         for (int frontIndx = 0; frontIndx < frontSize; ++frontIndx)
//         {
//             dist[frontIndx] = temp->index;
//             temp = temp->child;
//         }
//         assignCrowdingDistance(pop, obj_array, dist, frontSize, nobj);
//         Memory::Deallocate(dist);
//         Memory::Deallocate(obj_array, nobj);
//     }
// }

// /* Function to assign rank and crowding distance to a population of size pop_size*/
// void NSGA2:: assign_rank_and_crowding_distance (Population *newPop, int nobj)
//     {
//         int frontSize = 0;
//         int rank = 1;   
//         list *orig = nullptr;
//         list *cur = nullptr;
//         list *tempOrig = nullptr;
//         list *tempCur = nullptr;
        
//         initList(orig);
//         initList(cur);

//         tempOrig = orig;
//         for (int indIndx = 0; indIndx < newPop->size; ++indIndx)
//         {
//             insertNode (tempOrig, indIndx);
//             tempOrig = tempOrig->child;
//         }
//         while(orig->child)
//         {
//             if (!orig->child->child)
//             {
//                 newPop->ind[orig->child->index].rank = rank;
//                 newPop->ind[orig->child->index].crowd_dist = INF;
//                 break;
//             }
//             frontSize = 1;
//             tempOrig = orig->child;
//             insertNode (cur, tempOrig->index);
//             tempOrig = deleteNode (tempOrig);
//             Private::CurrentFrontPareto(newPop, orig, cur, frontSize, nobj);

//             tempCur = cur->child;
//             while (tempCur)
//             {
//                 newPop->ind[tempCur->index].rank = rank;
//                 tempCur = tempCur->child;
//             }
//             Private::assignCrowdingDistanceList (newPop, cur->child, frontSize, nobj);
//             tempCur = cur;
//             while (cur->child)
//             {
//                 tempCur = tempCur->child;
//                 tempCur = deleteNode (tempCur);
//             }
//             rank+=1;
//         }
        
//         DeleteList(orig);
//         DeleteList(cur);
        // Memory::DeallocateObj(orig);
        // Memory::DeallocateObj(cur);
//     }
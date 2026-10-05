#include "Sorting.h"
#include <windows.h>
#include <string>

void createXML(Population *pop, DataBank &data)
{
    const std::string filename = "C:\\Users\\vladi\\Desktop\\testCadFloOptimizator\\3\\Parametric Study 4\\in.xml"; /*тут должен быть полный путь*/
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Ошибка: Не удалось создать файл " << filename << std::endl;
        return;
    }

    file << "<?xml version=\"1.0\" encoding=\"utf-8\" ?>" << std::endl;
    file << "<InputData>" << std::endl;

    for (int pointIdx = 0; pointIdx < data.popsize; pointIdx++)
    {
        file << "  <Point index=\"0\" pointnum=\"" << (pointIdx + 1) << "\">" << std::endl;
        file << "    <InputParameters>" << std::endl;

        for (int paramIdx = 0; paramIdx < data.nreal; paramIdx++)
        {
            file << "      <InputParameter index=\"" << paramIdx
                 << "\" value=\"" << pop->ind[pointIdx].xreal[paramIdx] << "\"/>" << std::endl;
        }

        file << "    </InputParameters>" << std::endl;
        file << "  </Point>" << std::endl;
    }

    file << "</InputData>" << std::endl;
    file.close();
}

bool ReadOutputParameters(Population *pop, DataBank &data)
{
    const std::string filename = "C:\\Users\\vladi\\Desktop\\testCadFloOptimizator\\3\\Parametric Study 4\\out.xml"; /*тут должен быть полный путь*/
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
        return false;
    }

    std::string line;
    int outputIndex = 0;
    double currentValue = 0.0;
    int indIndx = 0;
    while (std::getline(file, line))
    {
        // Если встретили новую точку
        if (line.find("</Point>") != std::string::npos)
        {
            indIndx++;
            continue;
        }
        // Ищем строку с OutputParameter
        if (line.find("<OutputParameter") != std::string::npos)
        {
            // Ищем index="..."
            size_t indexPos = line.find("index=\"");
            if (indexPos != std::string::npos)
            {
                indexPos += 7; // Длина index="
                size_t indexEnd = line.find("\"", indexPos);
                int index = std::stoi(line.substr(indexPos, indexEnd - indexPos));
                // Ищем value="..."
                size_t valuePos = line.find("value=\"");
                if (valuePos != std::string::npos)
                {
                    valuePos += 7; // Длина value="
                    size_t valueEnd = line.find("\"", valuePos);
                    double value = std::stod(line.substr(valuePos, valueEnd - valuePos));
                    pop->ind[indIndx].obj[index] = value;
                }
            }
        }
    }
    file.close();
    remove(filename.c_str());
    return true;
}

void Signals::CADFlo(double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
    STARTUPINFOA si = {sizeof(si)};
    PROCESS_INFORMATION pi;
    std::string cmdLine = "\"C:\\Program Files\\SOLIDWORKS Corp\\SOLIDWORKS Flow Simulation\\binCFW\\NGP_ParametricStudyStarter.exe\" \"C:\\Users\\vladi\\Desktop\\testCadFloOptimizator\\3\\Parametric Study 4\\task.xml\" -start -wait";

    std::vector<char> cmdLineCopy(cmdLine.begin(), cmdLine.end());
    cmdLineCopy.push_back('\0');

    if (CreateProcessA(NULL, cmdLineCopy.data(), NULL, NULL,
                       FALSE, 0, NULL, NULL, &si, &pi))
    {

        std::cout << "Процесс запущен, PID: " << pi.dwProcessId << std::endl;
        WaitForSingleObject(pi.hProcess, INFINITE);

        DWORD exit_code;
        GetExitCodeProcess(pi.hProcess, &exit_code);
        std::cout << "Код выхода: " << exit_code << std::endl;

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
}

void Signals::osy(double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
    obj[0] = -(25.0 * pow((xreal[0] - 2.0), 2.0) + pow((xreal[1] - 2.0), 2.0) + pow((xreal[2] - 1.0), 2.0) + pow((xreal[3] - 4.0), 2.0) + pow((xreal[4] - 1.0), 2.0));
    obj[1] = xreal[0] * xreal[0] + xreal[1] * xreal[1] + xreal[2] * xreal[2] + xreal[3] * xreal[3] + xreal[4] * xreal[4] + xreal[5] * xreal[5];
    constr[0] = (xreal[0] + xreal[1]) - 2.0;
    constr[1] = 6.0 - (xreal[0] + xreal[1]);
    constr[2] = 2.0 - xreal[1] + xreal[0];
    constr[3] = 2.0 - xreal[0] + 3.0 * xreal[1];
    constr[4] = 4.0 - (pow((xreal[2] - 3.0), 2.0)) - xreal[3];
    constr[5] = (pow((xreal[4] - 3.0), 2.0)) + xreal[5] - 4.0;
}
void Signals::zdt5(double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
    int i, j;
    int u[11];
    int v[11];
    double f1, f2, g, h;
    for (i = 0; i < 11; i++)
    {
        u[i] = 0;
    }
    for (j = 0; j < 30; j++)
    {
        if (gene[0][j] == 1)
        {
            u[0]++;
        }
    }
    for (i = 1; i < 11; i++)
    {
        for (j = 0; j < 5; j++)
        {
            if (gene[i][j] == 1)
            {
                u[i]++;
            }
        }
    }
    f1 = 1.0 + u[0];
    for (i = 1; i < 11; i++)
    {
        if (u[i] < 5)
        {
            v[i] = 2 + u[i];
        }
        else
        {
            v[i] = 1;
        }
    }
    g = 0;
    for (i = 1; i < 11; i++)
    {
        g += v[i];
    }
    h = 1.0 / f1;
    f2 = g * h;
    obj[0] = f1;
    obj[1] = f2;
}

void Signals::zdt6(double *xreal, double *xbin, int **gene, double *obj, double *constr)
{
    double f1, f2, g, h;
    int i;
    f1 = 1.0 - (exp(-4.0 * xreal[0])) * pow((sin(4.0 * 3.14159265358979 * xreal[0])), 6.0);
    g = 0.0;
    for (i = 1; i < 10; i++)
    {
        g += xreal[i];
    }
    g = g / 9.0;
    g = pow(g, 0.25);
    g = 1.0 + 9.0 * g;
    h = 1.0 - pow((f1 / g), 2.0);
    f2 = g * h;
    obj[0] = f1;
    obj[1] = f2;
}

// void Signals::CADFlow(double *xreal, double *xbin, int **gene, double *obj, double *constr, std::string* file = nullptr)
// {
//     double f1, f2, g, h;

//     std::ifstream DataFile(*file);
//         if (!DataFile.is_open())
//         {
//             std::cerr << "No parameter file " << fileName << "in directory" << directory << " is found. Aborting" << std::endl;
//             std::terminate();
//             return;
//         }
//         std::string key, value, line;
//         int iterator = 0;
//         while (std::getline(DataFile, line))
//         {

//            std::string fragment = "value=";
//            size_t pos = str.find(fragment);

//            if (pos == std::string::npos) continue;

//            std::string value = str.substr(pos+5, 2);

//            obj[iterator] =std::stod(value);
//            iterator++;
//         }

//     obj[0] = f1;
//     obj[1] = f2;
// }

void Sorting::evaluateInd(Individual *ind, int ncon, ExternalSignal &signal)
{
    // signal.Operate(ind->xreal, ind->xbin, ind->gene, ind->obj, ind->constr);
    ind->constr_violation = 0.0;
    for (int constrIndx = 0; constrIndx < ncon; ++constrIndx)
        if (ind->constr[constrIndx] < 0.0)
            ind->constr_violation += ind->constr[constrIndx];
}

/* Процедура вычисления значений целевой функции и подсчета НЕудовлетворенных ограничений для популяции */
void Sorting::evaluatePop(Population *pop, DataBank &data, ExternalSignal &signal)
{
    createXML(pop, data);
    signal.Operate(pop->ind->xreal, pop->ind->xbin, pop->ind->gene, pop->ind->obj, pop->ind->constr);
    ReadOutputParameters(pop, data);
    for (int indIndx = 0; indIndx < data.popsize; ++indIndx)
        evaluateInd(&(pop->ind[indIndx]), data.ncon, signal);
}

/* Routine for usual non-domination checking
It will return the following values
1 if a dominates b
-1 if b dominates a
0 if both a and b are non-dominated */
int Sorting::checkDominance(Individual *a, Individual *b, DataBank &data)
{
    bool aDominance = false;
    bool bDominance = false;
    if (a->constr_violation < 0 && b->constr_violation < 0)
    {
        if (a->constr_violation > b->constr_violation)
            return (1);
        if (a->constr_violation < b->constr_violation)
            return (-1);
        else
            return (0);
    }
    if (a->constr_violation < 0 && b->constr_violation == 0)
        return (-1);
    if (a->constr_violation == 0 && b->constr_violation < 0)
        return (1);
    for (int objIndx = 0; objIndx < data.nobj; ++objIndx)
    {
        if (a->obj[objIndx] < b->obj[objIndx])
            aDominance = true;
        else if (a->obj[objIndx] > b->obj[objIndx])
            bDominance = true;
    }
    if (aDominance && !bDominance)
        return (1);
    else if (!aDominance && bDominance)
        return (-1);
    else
        return 0;
}

/* Actual implementation of the randomized quick sort used to sort a population based on a particular objective chosen */
void Sorting::qSortFrontObj(Population *pop, int *objArray, int objCount, int left, int right)
{
    int pivotPos = 0;
    int boundary = 0;
    double pivot = 0;
    if (left >= right)
        return;

    pivotPos = Math::randomRange(left, right);
    std::swap(objArray[right], objArray[pivotPos]);
    pivot = pop->ind[objArray[right]].obj[objCount];

    boundary = left - 1;
    for (int objArrayIndx = left; objArrayIndx < right; ++objArrayIndx)
    {
        if (pop->ind[objArray[objArrayIndx]].obj[objCount] > pivot)
            continue;
        std::swap(objArray[objArrayIndx], objArray[++boundary]);
    }

    pivotPos = boundary + 1;
    std::swap(objArray[right], objArray[pivotPos]);
    qSortFrontObj(pop, objArray, objCount, left, pivotPos - 1);
    qSortFrontObj(pop, objArray, objCount, pivotPos + 1, right);
}

/* Randomized quick sort routine to sort a population based on a particular objective chosen */
void Sorting::quickSortFrontObj(Population *pop, int *objArray, int objCount, int objArraySize)
{
    qSortFrontObj(pop, objArray, objCount, 0, objArraySize - 1);
}

/* Actual implementation of the randomized quick sort used to sort a population based on crowding distance */
void Sorting::qSortDist(Population *pop, int *dist, int left, int right)
{
    int pivotPos = 0;
    int boundary = 0;
    double pivot = 0;
    if (left >= right)
        return;

    pivotPos = Math::randomRange(left, right);
    std::swap(dist[right], dist[pivotPos]);
    pivot = pop->ind[dist[right]].crowd_dist;
    boundary = left - 1;
    for (int distIndx = left; distIndx < right; ++distIndx)
    {
        if (pop->ind[dist[distIndx]].crowd_dist > pivot)
            continue;
        std::swap(dist[++boundary], dist[distIndx]);
    }
    pivotPos = boundary + 1;
    std::swap(dist[right], dist[pivotPos]);
    qSortDist(pop, dist, left, pivotPos - 1);
    qSortDist(pop, dist, pivotPos + 1, right);
}

/* Randomized quick sort routine to sort a population based on crowding distance */
void Sorting::quickSortDist(Population *pop, int *dist, int frontSize)
{
    qSortDist(pop, dist, 0, frontSize - 1);
}
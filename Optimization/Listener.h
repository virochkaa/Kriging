#include <iostream>
#include <fstream>
#include <sstream>
#include "DataBank.h"
#include <unordered_map>

enum class DataType
{
    nreal,       // Число вещественных параметров
    nbin,        // Число бинарных параметров
    nobj,        // Число целевых функций
    ncon,        // Число ограничений для параметров
    popsize,     // Размер популяции
    pcross_real, // Вероятность SBX рекомбинации вещественных параметров
    pcross_bin,  // Вероятность SBX рекомбинации бинарных параметров
    pmut_real,   // Вероятность мутации вещественного параметра
    pmut_bin,    // Вероятность мутации бинарного параметра
    eta_c,       // Индекс распределения для вещественный переменных SBX рекомбинации
    eta_m,       // Индекс распределения для бинарных переменных SBX рекомбинации
    ngen,        // Количество поколений
    nbits,       // количество битов для i^{й} двоичной переменной
    min_realvar, // minimum value of i^{th} real variable
    max_realvar, // maximum value of i^{th} real variable
    min_binvar,  // minimum value of i^{th} binary variable
    max_binvar,  // maximum value of i^{th} binary variable
    none
};

class Listener
{
public:
    Listener() = default;
    ~Listener() = default;
    std::unordered_map<int, std::string> map;
    void DataCollection(const std::string &directory, const std::string &fileName, DataBank *Data)
    {
        std::ifstream DataFile(directory + "/" + fileName);
        if (!DataFile.is_open())
        {
            std::cerr << "No parameter file " << fileName << "in directory" << directory << " is found. Aborting" << std::endl;
            std::terminate();
            return;
        }
        std::string key, value, line;
        while (std::getline(DataFile, line))
        {
            if (line.empty() || line.find_first_not_of(" \t") == std::string::npos)
            {
                std::cout << "line" << std::endl;
                continue;
            }
            std::istringstream iss(line);
            if (iss >> key)
            {
                std::getline(iss >> std::ws, value);
            }
            // removeSpaces(value);
            if (key[0] == '/' && key[1] == '/')
                continue;
            if (key == "nreal")
            {
                std::istringstream iss(value);
                int _nreal = 0;
                iss >> _nreal;
                Data->nreal = _nreal;
                Memory::Allocate(Data->min_realvar, _nreal);
                Memory::Allocate(Data->max_realvar, _nreal);
                map[static_cast<unsigned short>(DataType::nreal)] = std::to_string(_nreal);
            }
            else if (key == "nbin")
            {
                std::istringstream iss(value);
                int _nbin = 0;
                iss >> _nbin;
                Data->nbin = _nbin;
                Memory::Allocate(Data->min_binvar, _nbin);
                Memory::Allocate(Data->max_binvar, _nbin);
                Memory::Allocate(Data->nbits, _nbin);
                map[static_cast<unsigned short>(DataType::nbin)] = std::to_string(_nbin);
            }
            else if (key == "ngen")
            {
                std::istringstream iss(value);
                int _ngen = 0;
                iss >> _ngen;
                Data->ngen = _ngen;
                map[static_cast<unsigned short>(DataType::ngen)] = std::to_string(_ngen);
            }
            else if (key == "nobj")
            {
                std::istringstream iss(value);
                int _nobj = 0;
                iss >> _nobj;
                Data->nobj = _nobj;
                map[static_cast<unsigned short>(DataType::nobj)] = std::to_string(_nobj);
            }
            else if (key == "ncon")
            {
                std::istringstream iss(value);
                int _ncon = 0;
                iss >> _ncon;
                Data->ncon = _ncon;
                map[static_cast<unsigned short>(DataType::ncon)] = std::to_string(_ncon);
            }
            else if (key == "popsize")
            {
                std::istringstream iss(value);
                int _popsize = 0;
                iss >> _popsize;
                Data->popsize = _popsize;
                map[static_cast<unsigned short>(DataType::popsize)] = std::to_string(_popsize);
            }
            else if (key == "pcross_real")
            {
                std::istringstream iss(value);
                double _pcross_real = 0;
                iss >> _pcross_real;
                Data->pcross_real = _pcross_real;
                map[static_cast<unsigned short>(DataType::pcross_real)] = std::to_string(_pcross_real);
            }
            else if (key == "pmut_real")
            {
                std::istringstream iss(value);
                double _pmut_real = 0;
                iss >> _pmut_real;
                Data->pmut_real = _pmut_real;
                map[static_cast<unsigned short>(DataType::pmut_real)] = std::to_string(_pmut_real);
            }
            else if (key == "pmut_bin")
            {
                std::istringstream iss(value);
                double _pmut_bin = 0;
                iss >> _pmut_bin;
                Data->pmut_bin = _pmut_bin;
                map[static_cast<unsigned short>(DataType::pmut_bin)] = std::to_string(_pmut_bin);
            }
            else if (key == "pcross_bin")
            {
                std::istringstream iss(value);
                double _pcross_bin = 0;
                iss >> _pcross_bin;
                Data->pcross_bin = _pcross_bin;
                map[static_cast<unsigned short>(DataType::pcross_bin)] = std::to_string(_pcross_bin);
            }
            else if (key == "eta_c")
            {
                std::istringstream iss(value);
                double _eta_c = 0;
                iss >> _eta_c;
                Data->eta_c = _eta_c;
                map[static_cast<unsigned short>(DataType::eta_c)] = std::to_string(_eta_c);
            }
            else if (key == "eta_m")
            {
                std::istringstream iss(value);
                double _eta_m = 0;
                iss >> _eta_m;
                Data->eta_m = _eta_m;
                map[static_cast<unsigned short>(DataType::eta_m)] = std::to_string(_eta_m);
            }
            else if (key == "nbits")
            {
                size_t _nbit = 0;
                for (int binIndx = 0; binIndx < Data->nbin; ++binIndx)
                {
                    std::getline(DataFile, value);
                    if (line.empty() || line.find_first_not_of(" \t") == std::string::npos)
                        continue;
                    std::istringstream iss(value);
                    iss >> _nbit;
                    std::cout << _nbit << std::endl;
                    Data->nbits[binIndx] = _nbit;
                    map.insert({static_cast<unsigned short>(DataType::nbin), std::to_string(_nbit)});
                }
            }
            else if (key == "MinMax_Realvar")
            {
                double _min_real = 0;
                double _max_real = 0;
                for (int realIndx = 0; realIndx < Data->nreal; ++realIndx)
                {
                    _min_real = 0;
                    _max_real = 0;
                    std::getline(DataFile, line);
                    if (line.empty() || line.find_first_not_of(" \t") == std::string::npos)
                        continue;
                    std::istringstream iss(line);

                    iss >> _min_real;
                    map.insert({static_cast<unsigned short>(DataType::min_realvar), std::to_string(_min_real)});
                    iss >> _max_real;
                    map.insert({static_cast<unsigned short>(DataType::max_realvar), std::to_string(_max_real)});
                    Data->min_realvar[realIndx] = _min_real;
                    Data->max_realvar[realIndx] = _max_real;
                }
            }
            else if (key == "MinMax_Binvar")
            {
                double _min_bin = 0;
                double _max_bin = 0;
                for (int binIndx = 0; binIndx < Data->nbin; ++binIndx)
                {
                    std::getline(DataFile, line);
                    if (line.empty() || line.find_first_not_of(" \t") == std::string::npos)
                        continue;
                    std::istringstream iss(line);

                    iss >> _min_bin;
                    map.insert({static_cast<unsigned short>(DataType::min_binvar), std::to_string(_min_bin)});
                    iss >> _max_bin;
                    map.insert({static_cast<unsigned short>(DataType::max_binvar), std::to_string(_max_bin)});
                    Data->min_binvar[binIndx] = _min_bin;
                    Data->max_binvar[binIndx] = _max_bin;
                }
            }
        }
    }
    double GetValue(DataType type)
    {
        std::string result = map[(int)type];
        return std::stod(result);
    }
    int GetInteger(DataType type)
    {
        std::string result = map[(int)type];
        return std::stoi(result);
    }
    bool GetChecker(DataType type)
    {
        std::string result = map[(int)type];
        if (result == "true" || result == " true" || result == "true ")
            return true;
        return false;
    }
};
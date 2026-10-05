#pragma once
#include <iostream>

namespace Memory
{
    //========= Память единичных классов ========= //

    template <typename T>
    void DeallocateObj(T *&obj)
    {
        if (!obj)
            return;
        delete obj;
        obj = nullptr;
    }
    template <typename T>
    void AllocateObj(T *&obj)
    {
        if (obj)
            DeallocateObj(obj);
        obj = new T();
    }

    //========= Память типизированных массиово ========= //

    template <typename T>
    void Deallocate(T *&obj)
    {
        if (!obj)
            return;
        delete[] obj;
        obj = nullptr;
    }
    template <typename T>
    void Deallocate(T **&obj, size_t _size)
    {
        if (_size == 0)
            return;
        if (!obj)
            return;
        for (size_t memIdx = 0; memIdx < _size; memIdx++)
        {
            Deallocate(obj[memIdx]);
        }
        delete[] obj;
        obj = nullptr;
    }

    template <typename T>
    void Allocate(T *&obj, size_t _size)
    {
        if (obj)
            Deallocate(obj);
        obj = new T[_size]();
    }

    template <typename T>
    void Allocate(T **&obj, size_t sizeUp, size_t sizeDown)
    {
        if (sizeUp == 0)
            return;
        if (obj)
            Deallocate(obj, sizeUp);
        obj = new T *[sizeUp];
        for (size_t memIdx = 0; memIdx < sizeUp; memIdx++)
        {
            obj[memIdx] = nullptr;
            Allocate(obj[memIdx], sizeDown);
        }
    }
    template <typename T>
    void Allocate(T **&obj, size_t sizeUp, size_t *sizeDown)
    {
        if (sizeUp == 0)
            return;
        if (obj)
            Deallocate(obj, sizeUp);
        obj = new T *[sizeUp];
        for (size_t memIdx = 0; memIdx < sizeUp; memIdx++)
        {
            obj[memIdx] = nullptr;
            Allocate(obj[memIdx], sizeDown[memIdx]);
        }
    }
}
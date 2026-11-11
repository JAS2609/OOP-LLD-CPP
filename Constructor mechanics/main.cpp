#include <iostream>
#include <stdexcept>
using namespace std;

class DynamicArray
{
private:
    int *arr;
    int size;

public:
    DynamicArray(int size)
    {
        if (size <= 0)
            throw std::invalid_argument("Size must be greater than 0");
        this->size = size;
        arr = new int[size];
    }

    ~DynamicArray()
    {
        delete[] arr;
    }

    void setValue(int index, int value)
    {
        if (index >= 0 && index < size)
        {
            arr[index] = value;
        }
    }
    int getSize() const
    {
        return size;
    }
    int getValue(int index) const
    {
        if (index >= 0 && index < size)
        {
            return arr[index];
        }
        return -1;
    }
    DynamicArray(const DynamicArray &other)
    {
        size = other.size;
        arr = new int[size];
        for (int i = 0; i < size; i++)
        {
            arr[i] = other.arr[i];
        }
    }
    DynamicArray &operator=(const DynamicArray &other)
    {
        if (this == &other)
        {
            return *this;
        }
        int *newArr = new int[other.size];
        for (int i = 0; i < other.size; i++)
        {
            newArr[i] = other.arr[i];
        }
        delete[] arr;
        arr = newArr;
        size = other.size;

        return *this;
    }

    DynamicArray(DynamicArray &&other) noexcept
        : arr(other.arr), size(other.size)
    {

        other.arr = nullptr;
        other.size = 0;
    };
    DynamicArray &operator=(DynamicArray &&other) noexcept
    {

        if (this != &other)
        {
            delete[] arr;
            arr = other.arr;
            size = other.size;
            other.arr = nullptr;
            other.size = 0;
        }
        return *this;
    };
};
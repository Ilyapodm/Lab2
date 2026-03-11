#pragma once

template <typename T>
class DynamicArray {
public:
    DynamicArray();
    DynamicArray(int size);
    DynamicArray(T* items, int size);
    DynamicArray(const DynamicArray<T> &other);
    ~DynamicArray();

    DynamicArray<T>& operator=(const DynamicArray<T> &other);

    const T& get(int index) const;
    
    int get_size() const;
    int get_capacity() const;

    void set(int index, const T& value);
    
    void resize(int new_size);
    
private:
    T *data;
    int size;      // means the number of elements + reachable ceils of array in memory
    int capacity;  // all allocated ceils of array in memory
};

#include "dynamic_array.tpp"
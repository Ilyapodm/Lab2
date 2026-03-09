#pragma once

template <typename T>
class DynamicArray {
public:
    DynamicArray();
    DynamicArray(int size);
    DynamicArray(T* items, int size);
    DynamicArray(const DynamicArray<T> &other);

    DynamicArray<T>& operator=(const DynamicArray<T> &other);

    const T& get(int index) const;
    
    int get_size() const;

    void set(int index, const T& value);
    void resize(int new_size);

    ~DynamicArray();

private:
    T *data;
    int size;
};

#include "dynamic_array.tpp"
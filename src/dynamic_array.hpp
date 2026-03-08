// #ifndef DYNAMIC_ARRAY_HPP
// #define DYNAMIC_ARRAY_HPP
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
    int get_capacity() const;

    void set(int index, const T& value);
    void resize(int new_size);

    ~DynamicArray();

private:
    T *data;
    int size;
    int capacity;  // BUG в задании про capacity ничего не сказано, можно ли сделать?
};

#include "dynamic_array.tpp"

//#endif  // DYNAMIC_ARRAY_HPP
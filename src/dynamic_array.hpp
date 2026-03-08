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

    T get(int index) const;
    int get_size() const;
    int get_capacity() const;

    void set(int index, T value);
    void resize(int new_size);

private:
    T *data;
    int size;
    int capacity;
};

#include "dynamic_array.tpp"

//#endif  // DYNAMIC_ARRAY_HPP
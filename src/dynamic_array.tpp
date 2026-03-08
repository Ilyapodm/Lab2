#pragma once

#include "dynamic_array.hpp"
#include <stdexcept>

template <typename T> 
DynamicArray<T>::DynamicArray() : size{0}, capacity{0} {
    data = nullptr;
}

template <typename T>
DynamicArray<T>::DynamicArray(int size) : size{size}, capacity{size} {
    data = new T[capacity];
}

template <typename T>
DynamicArray<T>::DynamicArray(T *items, int size) : size{size}, capacity{size} {
    data = new T[capacity];

    for (int i = 0; i < size; i++) {
        data[i] = items[i];
    }
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray<T> &other) : size{other.size}, capacity{other.capacity} {
    data = new T[capacity];

    for (int i = 0; i < size; i++) {
        data[i] = other.data[i];
    }
}

template <typename T>
T DynamicArray<T>::get(int index) const {
    if (index >= size || index < -size) {
        throw std::out_of_range("Index out of range");
    }

    if (index >= 0) 
        return data[index];
    else
        return data[size + index];  // works with negative indexes
}

// PERF git_size и git_capacity являются одинаковыми абсолютно для всех шаблонов, но все равно у каждого будет свой
template <typename T>
int DynamicArray<T>::get_size() const{
    return size;
}

template <typename T>
int DynamicArray<T>::get_capacity() const {
    return capacity;
}

template <typename T>
void DynamicArray<T>::set(int index, T value) {
    if (index >= size || index < -size) {
        throw std::out_of_range("Index out of range");
    }

    if (index >= 0) 
        data[index] = value;
    else
        data[size + index] = value; // works with negative indexes
}

template <typename T>
void DynamicArray<T>::resize(int new_size) {
    if (new_size == capacity)  // nothing to do, already needed capacity 
        return;

    capacity = new_size;

    T *new_data = new T[capacity];

    int items = (size < capacity) ? size : capacity;  // number of elements in the new data

    for (int i = 0; i < items; i++) {
        new_data[i] = data[i];
    }

    delete[] data;
    data = new_data;
    size = items;
}
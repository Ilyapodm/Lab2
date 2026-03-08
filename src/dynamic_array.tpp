#pragma once
#include "dynamic_array.hpp"
#include <stdexcept>

template <typename T> 
DynamicArray<T>::DynamicArray() : size{0}, capacity{0} {
    data = nullptr;
}

template <typename T>
DynamicArray<T>::DynamicArray(int size) : size{size}, capacity{size} {
    if (size < 0)
        throw std::invalid_argument("DynamicArray: size cannot be negative");

    data = (size > 0) ? new T[capacity]{} : nullptr;  // do the initialization of items
}

template <typename T>
DynamicArray<T>::DynamicArray(T *items, int size) : size{size}, capacity{size} {
    if (size < 0)
        throw std::invalid_argument("DynamicArray: size cannot be negative");

    if (items == nullptr && size > 0)
        throw std::invalid_argument("DynamicArray: items is nullptr");

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

DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray<T> &other) {
    if (&other == this) 
        return *this;

    T *new_data = new T[other.capacity];

    for (int i = 0; i < other.size; i++) {
        new_data[i] = other.data[i];
    }

    delete[] data;
    size = other.size;
    capacity = other.capacity;
    data = new_data;

    return *this;
}

template <typename T>
const T& DynamicArray<T>::get(int index) const {
    if (index >= size || index < 0) {
        throw std::out_of_range("get: Index out of range");
    }
    
    return data[index];
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
void DynamicArray<T>::set(int index, const T& value) {
    if (index >= size || index < 0) {
        throw std::out_of_range("set: Index out of range");
    }

        data[index] = value;
}

template <typename T>
void DynamicArray<T>::resize(int new_size) {
    if (new_size < 0)
        throw std::invalid_argument("resize: new_size cannot be negative");

    if (new_size == capacity)  // nothing to do, already needed capacity 
        return;

    T* new_data = (new_size > 0) ? new T[new_size] : nullptr;

    int items = (size < new_size) ? size : new_size;  // number of elements in the new data

    for (int i = 0; i < items; i++) {
        new_data[i] = data[i];
    }

    delete[] data;
    data = new_data;
    size = items;
    capacity = new_size;
}

template <typename T>
DynamicArray<T>::~DynamicArray() {
    delete []data;
}
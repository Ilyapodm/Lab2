// #ifndef SEQUENCE_TPP
// #define SEQUENCE_TPP

#pragma once

#include "dynamic_array.hpp"
#include "sequence.hpp"
#include <stdexcept>

template <typename T>
ArraySequence<T>::ArraySequence() : size{0} {
    array = new DynamicArray<T>;
}

template <typename T>
ArraySequence<T>::ArraySequence(T *items, int size) : size{size} {
    array = new DynamicArray<T>();  //TODO сделать через конструктор дин массива
}

template <typename T>
ArraySequence<T>::ArraySequence(const DynamicArray<T> &array) {
    
}

// #endif // SEQUENCE_TPP
#pragma once

#include "sequence.hpp"
#include "dynamic_array.hpp"
#include "linked_list.hpp"
#include <stdexcept>

template <typename T>
ArraySequence<T>::ArraySequence() : size{0} {
    array = new DynamicArray<T>();
}

template <typename T>
ArraySequence<T>::ArraySequence(T *items, int size) : size{size} {
    array = new DynamicArray<T>();  //TODO сделать через конструктор дин массива
}

template <typename T>
ArraySequence<T>::ArraySequence(const DynamicArray<T> &array) {
    
}


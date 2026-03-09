#pragma once

#include "sequence.hpp"
#include "dynamic_array.hpp"
#include "linked_list.hpp"
#include <stdexcept>

template <typename T>
ArraySequence<T>::ArraySequence() {
    array = new DynamicArray<T>();
}

template <typename T>
ArraySequence<T>::ArraySequence(T *items, int size) {
    array = new DynamicArray<T>(items, size);  //TODO сделать через конструктор дин массива
}

template <typename T>
ArraySequence<T>::ArraySequence(const DynamicArray<T> &array) {
    array = new DynamicArray<T>(array);
}

template <typename T>
const T& ArraySequence<T>::get_first() const {
    return array->get(0);
}

template <typename T>
const T& ArraySequence<T>::get_last() const {
    return array->get(array->get_size() - 1);
}

template <typename T>
const T& ArraySequence<T>::get(int index) const {
    return array->get(index);
}

template <typename T>
int ArraySequence<T>::get_size() const {
    return array->get_size();
}

template <typename T>
Sequence<T>* ArraySequence<T>::get_subsequence(int start_index, int end_index) const {
    if (start_index < 0 || end_index >= array->get_size() || start_index > end_index || array->get_size() == 0) 
        throw std::out_of_range("get_subsequence: index out of range");

    ArraySequence<T>* result = new ArraySequence<T>();

    try {
        for (int i = start_index; i <= end_index; i++)
            result->append(array->get(i));
    } catch (...) {
        delete result;
        throw;
    }

    return result;
}

template <typename T>
Sequence<T>* ArraySequence<T>::append(const T& item) {
    array->resize(array->get_size() + 1);
    array->set(array->get_size(),  item);
    return this;
}

template <typename T>
Sequence<T>* ArraySequence<T>::prepend(const T& item) {
    array->resize(array->get_size() + 1);
    for (int i = array->get_size() - 1; i > 0; i--) {
        array->set(i, array->get(i - 1));
    }
    array->set(0,  item);
    return this;
}

template <typename T>
Sequence<T>* ArraySequence<T>::insert_at(const T& item, int index) {
    if (index < 0 || index > array->get_size())
        throw std::out_of_range("insert_at: index out of range");

    if (index == array->get_size()) {
        this->append(item);
        return this;
    }

    if (index == 0) {
        this->prepend(item);
        return this;
    }

    
}
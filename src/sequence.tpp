#pragma once

#include "sequence.hpp"
#include "dynamic_array.hpp"
// #include "linked_list.hpp"
#include <stdexcept>

template <typename T>
ArraySequence<T>::ArraySequence() {
    array = new DynamicArray<T>();
}

template <typename T>
ArraySequence<T>::ArraySequence(T *items, int size) {
    array = new DynamicArray<T>(items, size); 
}

template <typename T>
ArraySequence<T>::ArraySequence(const DynamicArray<T> &array) {
    array = new DynamicArray<T>(array);
}

template <typename T>
ArraySequence<T>::ArraySequence(const ArraySequence<T> &other) {
    array = new DynamicArray<T>(*other.array);
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
    if (start_index < 0 || end_index >= array->get_size() || start_index > end_index)  // cover size = 0
        throw std::out_of_range("get_subsequence: index out of range");

    // need this method because we don't know what (mutable/immutable) we are working with
    ArraySequence<T> *result = this->empty_sequence();

    try {
        int total_size = end_index - start_index + 1;  

        result->array->resize(total_size);  // resize all needed space one time

        for (int i = start_index; i <= end_index; i++){
            // do not use append because it can create another copy for immutable
            result->array->set(result->array->get_size() - 1, array->get(i));
        }
    } catch (...) {
        delete result;
        throw;
    }

    return result;
}

template <typename T>
Sequence<T>* ArraySequence<T>::append(const T& item) {
    ArraySequence<T> *inst = this->instance();  // make a copy

    try {
        inst->array->resize(inst->array->get_size() + 1);  
        inst->array->set(inst->array->get_size() - 1,  item);  // set at size-1 because size is increased on previous string   
    } catch (...) {
        delete inst;
        throw;
    }
    
    return inst;
}

template <typename T>
Sequence<T>* ArraySequence<T>::prepend(const T& item) {
    ArraySequence<T> *inst = this->instance();

    try{
        inst->array->resize(inst->array->get_size() + 1);
        
        for (int dst = inst->array->get_size() - 1; dst > 0; dst--) {
            inst->array->set(dst, inst->array->get(dst - 1));
        }
        inst->array->set(0,  item);
    } catch (...) {
        delete inst;
        throw;
    }

    return inst;
}

template <typename T>
Sequence<T>* ArraySequence<T>::insert_at(const T& item, int index) {
    if (index < 0 || index > array->get_size())
        throw std::out_of_range("insert_at: index out of range");

    ArraySequence<T> *inst = this->instance();

    try {
        inst->array->resize(inst->array->get_size() + 1);

        for (int dst = inst->get_size() - 1; dst > index; dst--) {
            inst->array->set(dst, inst->array->get(dst - 1));
        }

        inst->array->set(index, item);
    } catch (...) {
        delete inst;
        throw;
    }
    
    return inst;
}

template <typename T>
Sequence<T>* ArraySequence<T>::concat(Sequence<T> *other) const {
    ArraySequence<T> *result = this->empty_sequence();

    try {
        int total_size = this->get_size() + other->get_size();
        result->array->resize(total_size);  // resize all needed space at one time

        for (int i = 0; i < this->get_size(); i++) 
            // do not use append because it can create another copy for immutable
            result->array->set(i, array->get(i));
        
        for (int i = 0; i < other->get_size(); i++) 
            result->array->set(this->get_size() + i, other->get(i));  // have already placed get_size - 1 spaces
        
    } catch (...) {
        delete result;
        throw;
    }
    
    return result;
}
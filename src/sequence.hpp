#pragma once

#include "option.hpp"
#include "ienumerator.hpp"

template <typename T>
class Sequence {
public:
    virtual const T& get_first() const = 0;  // pure virtual method (have to be realized in child-classes)
    virtual const T& get_last() const = 0;
    virtual const T& get(int index) const = 0;

    Option<T> try_get_first() const;
    Option<T> try_get_last() const;
    Option<T> try_get(int index) const;

    virtual int get_size() const = 0;

    virtual Sequence<T>* get_subsequence(int start_index, int end_index) const = 0;

    virtual Sequence<T>* append(const T& item) = 0;
    virtual Sequence<T>* prepend(const T& item) = 0;
    virtual Sequence<T>* insert_at(const T& item, int index) = 0;
    virtual Sequence<T>* concat(Sequence<T> *list) const = 0;

    virtual Sequence<T>* Map(T (*mapper)(const T& element)) = 0;
    virtual Sequence<T>* Where(bool (*predicate)(const T& element)) = 0;
    virtual T Reduce(T (*reduce_func)(const T& first_element, const T& second_element), const T& start_element) = 0;

    virtual IEnumerator<T>* get_enumerator() const = 0;

    virtual ~Sequence() {}
};

#include "sequence.tpp"
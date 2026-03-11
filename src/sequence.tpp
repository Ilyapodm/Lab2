#pragma once

#include "sequence.hpp"
#include "option.hpp"
#include <stdexcept>

// using const links for the object of exception
template <typename T>
Option<T> Sequence<T>::try_get_first() const {
    try {
        return Option<T>::Some(this->get_first());
    } catch (const std::out_of_range&) {
        return Option<T>::None();
    }
}

template <typename T>
Option<T> Sequence<T>::try_get_last() const {
    try {
        return Option<T>::Some(this->get_last());
    } catch (const std::out_of_range&) {
        return Option<T>::None();
    }
}

template <typename T>
Option<T> Sequence<T>::try_get(int index) const {
    try {
        return Option<T>::Some(this->get(index));
    } catch (const std::out_of_range&) {
        return Option<T>::None();
    }
}
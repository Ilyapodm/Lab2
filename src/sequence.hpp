#pragma once

#include "dynamic_array.hpp"
#include "linked_list.hpp"
#include "option.hpp"

//TODO написать все методы для linked list
//TODO реализовать map, where, reduce
//TODO сделать Option для try_get... 

template <typename T>
class Sequence {
public:
    virtual const T& get_first() const = 0;  // pure virtual method
    virtual const T& get_last() const = 0;
    virtual const T& get(int index) const = 0;

    // TODO try_get... через Option<T>

    virtual Sequence<T>* get_subsequence(int start_index, int end_index) const = 0;

    virtual int get_size() const = 0;

    virtual Sequence<T>* append(const T& item) = 0;
    virtual Sequence<T>* prepend(const T& item) = 0;
    virtual Sequence<T>* insert_at(const T& item, int index) = 0;
    virtual Sequence<T>* concat(Sequence<T> *list) const = 0;

    virtual Sequence<T>* Map(T (*mapper)(const T& element)) = 0;
    virtual Sequence<T>* Where(bool (*predicate)(const T& element)) = 0;
    virtual T Reduce(T (*reduce_func)(const T& first_element, const T& second_element), const T& start_element) = 0;

    // IEnumerator<T> GetEnumerator();

    virtual ~Sequence() {}
};

template <typename T>
class ArraySequence : public Sequence<T> {
public:
    ArraySequence();
    ArraySequence(T *items, int size);  // copy from given array
    ArraySequence (const DynamicArray<T> &array);
    ArraySequence(const ArraySequence<T> &other);

    virtual ArraySequence<T>* instance() const = 0;  // returns who will be changed (copy or actually the object)

    // need it because ArraySequence class is abstract (can't create objects), and in  
    // the methods we don't know mutable or immutable objects we are working with
    virtual ArraySequence<T>* empty_sequence() const = 0;  // returns an empty Mutable or Immutable (array sequence)

    const T& get_first() const override;  
    const T& get_last() const override;
    const T& get(int index) const override;

    // TODO try_get... через Option

    int get_size() const override;
    
    Sequence<T>* get_subsequence(int start_index, int end_index) const override;

    Sequence<T>* append(const T& item) override;
    Sequence<T>* prepend(const T& item) override;
    Sequence<T>* insert_at(const T& item, int index) override;
    Sequence <T>* concat(Sequence<T> *other) const override;

    Sequence<T>* Map(T (*mapper)(const T& element)) override;
    Sequence<T>* Where(bool (*predicate)(const T& element)) override;
    T Reduce(T (*reduce_func)(const T& first_element, const T& second_element), const T& start_element) override;

    //TODO Итератор

    ~ArraySequence() {
        delete array;
    }

protected:
    DynamicArray<T> *array;
};

template <typename T>
class ListSequence : public Sequence<T> {
public:
    ListSequence();
    ListSequence(T *items, int size);  //copy from given list
    ListSequence (const LinkedList<T> &list);
    ListSequence(const ListSequence<T>& other);  //TODO реализовать конструктор копирования
    
    virtual ListSequence<T>* instance() = 0;  // returns who will be changed (copy or actually the object) 

    // need it because ArraySequence class is abstract (can't create objects), and in  
    // the methods we don't know mutable or immutable objects we are working with
    virtual ListSequence<T>* empty_sequence() = 0;  // returns an empty Mutable or Immutable list sequence

    const T& get_first() const override;  
    const T& get_last() const override;
    const T& get(int index) const override;

    // TODO try_get... через Option

    int get_size() const override;

    Sequence<T>* get_subsequence(int start_index, int end_index) const override;

    Sequence<T>* append(T item) override;
    Sequence<T>* prepend(T item) override;
    Sequence<T>* insert_at(T item, int index) override;
    Sequence <T>* concat(Sequence<T> *list) const override;

    Sequence<T>* Map(T (*mapper)(const T& element)) override;
    Sequence<T>* Where(bool (*predicate)(const T& element)) override;
    T Reduce(T (*reduce_func)(const T& first_element, const T& second_element), const T& start_element) override;

    // TODO Итератор

    ~ListSequence() {
        delete list;
    }

protected:
    LinkedList<T> *list;
}; 

template <typename T>
class MutableArraySequence : public ArraySequence<T> {
public:
    MutableArraySequence() : ArraySequence<T>() {}
    MutableArraySequence(T *items, int size) : ArraySequence<T>(items, size) {}
    MutableArraySequence(const DynamicArray<T> &array) : ArraySequence<T>(array) {}
    MutableArraySequence(const ArraySequence<T> &other) : ArraySequence<T>(other) {}
protected:
    ArraySequence<T>* instance() override {
        return this;  // return actually the object
    }

    ArraySequence<T>* empty_sequence() const override {
        return new MutableArraySequence<T>();
    }
};

template <typename T>
class ImmutableArraySequence : public ArraySequence<T> {
public:
    ImmutableArraySequence() : ArraySequence<T>() {}
    ImmutableArraySequence(T *items, int size) : ArraySequence<T>(items, size) {}
    ImmutableArraySequence(const DynamicArray<T> &array) : ArraySequence<T>(array) {}
    ImmutableArraySequence(const ArraySequence<T> &other) : ArraySequence<T>(other) {}
protected:
    ArraySequence<T>* instance() override {
        return new ImmutableArraySequence<T>(*this);  // return the copy
    }

    ArraySequence<T>* empty_sequence() const override {
        return new ImmutableArraySequence<T>();
    }
};

// template <typename T>
// class MutableListSequence : public ListSequence<T> {
// public:
//     ListSequence<T>* instance() override {
//         return this;
//     }

// };

// template <typename T>
// class ImmutableListSequence : public ListSequence<T> {
// public:
//     ListSequence<T>* instance() override {
//         return this->clone();
//     }

// };

class Bit {
public:
    Bit() : value{false} {}
    Bit(bool value) : value{value} {}
    Bit(int value) : value{value != 0} {}

    // AND
    Bit operator&(const Bit& other) const {
        return Bit(value && other.value);
    }

    // OR
    Bit operator|(const Bit& other) const {
        return Bit(value || other.value);
    }

    // XOR
    Bit operator^(const Bit& other) const {
        return Bit(value != other.value);
    }

    // NOT
    Bit operator~() const {
        return Bit(!value);
    }

    bool operator==(const Bit& other) const {
        return value == other.value;
    }

    bool operator!=(const Bit& other) const {
        return value != other.value;
    }

private:
    bool value;
};

class BitSequence {
public:
    // TODO конструктор

    BitSequence* operator&(const BitSequence& other) const;
    BitSequence* operator|(const BitSequence& other) const;

private:
    Sequence<Bit> *sequence;
};

#include "sequence.tpp"
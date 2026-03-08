#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

#include "dynamic_array.hpp"
#include "linked_list.hpp"

template <typename T>
class Sequence {
public:
    virtual const T& get_first() const = 0;  // pure virtual method
    virtual const T& get_last() const = 0;
    virtual const T& get(int index) const = 0;

    // TODO try_get... через Option<T>

    virtual Sequence<T>* get_subsequence(int start_index, int end_index) const = 0;

    virtual int get_size() const = 0;

    virtual Sequence<T>* append(T item) = 0;
    virtual Sequence<T>* prepend(T item) = 0;
    virtual Sequence<T>* insert_at(T item, int index) = 0;
    virtual Sequence <T>* concat(Sequence <T> *list) = 0;

    // TODO настроить правильные объявления для методов
    // Sequence <T>* Map(T (*)(T));
    // Sequence <T>* Where(bool (*)(T));
    // Sequence <T>* Reduce(T2 (*)(T2,T));
    // Option<T> GetFirst(bool (*)(T) = 0);
    // Option<T> GetLast(bool (*)(T) = 0);
    // IEnumerator<T> GetEnumerator();
};

template <typename T>
class ArraySequence : public Sequence<T> {
public:
    ArraySequence(T *items, int count);  // copy from given array
    ArraySequence();
    ArraySequence (const LinkedList<T> &list);

    ~ArraySequence();

    const T& get_first() const override;  
    const T& get_last() const override;
    const T& get(int index) const override;

    // TODO try_get... через Option

    int get_size() const override;

    Sequence<T>* get_subsequence(int start_index, int end_index) override;

    Sequence<T>* append(T item) override;
    Sequence<T>* prepend(T item) override;
    Sequence<T>* insert_at(T item, int index) override;
    Sequence <T>* concat(Sequence<T> *list) override;

    // TODO map, where, reduce здесь сделать от sequence + итератор

protected:
    DynamicArray<T> *array;
    int size;
};

template <typename T>
class ListSequence : public Sequence<T> {
public:
    ListSequence(T *items, int count);  //copy from given list
    ListSequence();
    ListSequence (const LinkedList<T> &list);
    ~ListSequence(); // TODO подумать че с ним делать

    const T& get_first() const override;  
    const T& get_last() const override;
    const T& get(int index) const override;

    // TODO try_get... через Option

    int get_size() const override;

    Sequence<T>* get_subsequence(int start_index, int end_index) override;

    Sequence<T>* append(T item) override;
    Sequence<T>* prepend(T item) override;
    Sequence<T>* insert_at(T item, int index) override;
    Sequence <T>* concat(Sequence<T> *list) override;

    // TODO map, where, reduce здесь сделать от sequence + итератор

protected:
    // TODO можно ли добавить size в список?
    LinkedList<T> *list;
}; 

template <typename T>
class MutableArraySequence : ArraySequence<T> {
public:

};

template <typename T>
class ImmutableArraySequence : ArraySequence<T> {
public:

};

template <typename T>
class MutableListSequence : ListSequence<T> {
public:

};

template <typename T>
class ImmutableListSequence : ListSequence<T> {
public:

};

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
    
private:
};

#include "sequence.tpp"

#endif  //SEQUENCE_HPP
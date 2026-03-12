#pragma once

#include "ienumerator.hpp"

template <typename T>
class LinkedList {
private:  // here for enumerator
    struct Node {
        T data;
        Node *next;
    };

public:
    LinkedList();
    LinkedList(T *items, int size);
    LinkedList(const LinkedList<T> &other);
    ~LinkedList();

    LinkedList& operator=(const LinkedList& other);
    
    int get_length() const;

    const T& get_first() const;
    const T& get_last() const ;
    const T& get(int index) const;

    LinkedList<T>* get_sublist(int start_index, int end_index) const;
    
    void set(int index, const T &item);
    void append(const T &item); 
    void prepend(const T &item);
    void insert_at(const T &item, int index);
    T remove_at(int index);

    void filter(bool (*predicate)(const T&));  // "where" in list_sequence doesn't have the needed access (for not being O(n^2))

    LinkedList<T>* concat(const LinkedList<T> *other) const; 

    // caller owns, must delete
    IEnumerator<T>* get_enumerator() const;

    // nested class for enumerator
    class ListEnumerator : public IEnumerator<T> {
    public:
        ListEnumerator(const LinkedList<T> *linked_list) : linked_list{linked_list}, current_node{nullptr}, index{-1} {}

        bool move_next() override;  // move to next element
                
        const T& get_current() const override;  // get current item

        void set_current(const T &value) override;  // set current item

        void reset() override;  // move to the beginning

    private:
        int index;
        Node *current_node;  // we can change Node, but linked list will stay unchanged (length, tail, head will be the same)
        const LinkedList<T> *linked_list;
    };

private:
    int length; 

    Node *head;
    Node *tail;
};

#include "linked_list.tpp"
#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

template <typename T>
class LinkedList {
public:
    LinkedList (T* items, int count);
    LinkedList ();
    LinkedList (const LinkedList <T> &list);
    
    T get_first();
    T get_last();
    T get(int index);
    LinkedList<T>* get_sublist(int start_index, int end_index);
    int get_length();

    void append(T item); 
    void prepend(T item);
    void insert_at(T item, int index);
    LinkedList<T>* concat(LinkedList<T> *list); 
};

#include "linked_list.tpp"

#endif  // LINKED_LIST_HPP
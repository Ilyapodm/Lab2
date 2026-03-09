#pragma once

template <typename T>
class LinkedList {
public:
    LinkedList();
    LinkedList(T *items, int size);
    LinkedList(const LinkedList<T> &list);

    LinkedList& operator=(const LinkedList& other);
    
    int get_length() const;

    const T& get_first() const;
    const T& get_last() const ;
    const T& get(int index) const;

    LinkedList<T>* get_sublist(int start_index, int end_index) const;
    
    void append(const T& item); 
    void prepend(const T& item);
    void insert_at(const T& item, int index);
    LinkedList<T>* concat(const LinkedList<T> *other) const; 

    ~LinkedList();

private:
    struct Node {
        T data;
        Node *next;
    };

    int length; 

    Node *head;
    Node *tail;
};

#include "linked_list.tpp"
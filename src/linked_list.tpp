#include <stdexcept>

template <typename T>
LinkedList<T>::LinkedList() : head{nullptr}, tail{nullptr}, length{0} {}

template <typename T>
LinkedList<T>::LinkedList(T *items, int size) : head{nullptr}, tail{nullptr}, length{size} {
    if (length <= 0) {
        length = 0;
        return;
    }

    // create first Node (head and tail at the same time)
    head = new Node{.data = items[0], .next = nullptr};
    tail = head;

    for (int i = 1; i < size; i++) {
        tail->next = new Node{.data = items[i], .next = nullptr};  // create new Node (tail's next)
        tail = tail->next;  // move tail to just created Node
    }
}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList <T> &other) : head{nullptr}, tail{nullptr}, length{other.length} {
    if (length == 0) 
        return;

    // create first Node (head and tail at the same time)
    head = new Node{.data = other.head->data, .next = nullptr};
    tail = head;

    Node *current_other_node = other.head;  
    for (int i = 1; i < length; i++) {
        current_other_node = current_other_node->next;  // move to next other's Node

        tail->next = new Node{.data = current_other_node->data, .next = nullptr};  // create new Node (tail's next)
        tail = tail->next;  // move tail to just created Node
    }
}

template <class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& other) {
    if (this == &other) return *this; // no self assignment
    
    // delete old Nodes
    Node* current = head;
    while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }
    
    // copy
    head = tail = nullptr;
    length = 0;
    Node* current_node = other.head;
    while (current_node != nullptr) {
        append(current_node->data);
        current_node = current_node->next;
    }
    return *this;
}

template <typename T>
const T& LinkedList<T>::get_first() const {
    if (head == nullptr) 
        throw std::out_of_range("get_first: The list is empty");
    

    return head->data;
}

template <typename T>
const T& LinkedList<T>::get_last() const {
    if (tail == nullptr) 
        throw std::out_of_range("get_last: The list is empty");
    

    return tail->data;
}

template <typename T>
const T& LinkedList<T>::get(int index) const {
    if (index < 0 || index >= length)
        throw std::out_of_range("get: Index out of range");

    Node *current_node = head;

    for (int i = 0; i < index; i++) {
        current_node = current_node->next;
    }

    return current_node->data;
}

template <typename T>
int LinkedList<T>::get_length() const {
    return length;
}

template <typename T>
LinkedList<T>* LinkedList<T>::get_sublist(int start_index, int end_index) const {
    if (head == nullptr)
        throw std::out_of_range("get_sublist: The list is empty");
    if (start_index < 0 || end_index >= length)
        throw std::out_of_range("get_sublist: Index out of range");
    if (start_index > end_index)
        throw std::out_of_range("get_sublist: Start index can't be greater than end index");

    LinkedList<T>* result = new LinkedList<T>();

    Node* current_node = head;
    for (int i = 0; i < start_index; i++)
        current_node = current_node->next;

    for (int i = start_index; i <= end_index; i++) {
        result->append(current_node->data);
        current_node = current_node->next;
    }

    return result;
}


template <typename T>
void LinkedList<T>::append(const T& item) {
    
    Node *new_node = new Node{.data = item, .next = nullptr};

    if (head == nullptr) {
        head = new_node;
        tail = head;
    }
    else {
        tail->next = new_node;
        tail = tail->next;
    }

    length++;
}

template <typename T>
void LinkedList<T>::prepend(const T& item) {
    Node *new_node = new Node{.data = item, .next = nullptr};

    if (head == nullptr) {
        head = new_node;
        tail = head;
    }
    else {
        new_node->next = head;
        head = new_node;
    }

    length++;
}

template <typename T>
void LinkedList<T>::insert_at(const T& item, int index) {
    // inserting at the end is available (the same as append)
    if (index < 0 || index > length) {
        throw std::out_of_range("insert_at: Index out of range");
    }

    if (index == 0) {
        this->prepend(item);
        return;
    }

    if (index == length) {
        this->append(item);
        return;
    }

    Node *new_node = new Node{.data = item, .next = nullptr};
    Node *current_node = head;

    for (int i = 0; i < index - 1; i++) {
        current_node = current_node->next;
    }

    Node *next_node = current_node->next;
    current_node->next = new_node;
    new_node->next = next_node;

    length++;
}

template <typename T>
LinkedList<T>* LinkedList<T>::concat(LinkedList<T> *other) const {
    if (other == nullptr) {
        throw std::invalid_argument("concat: Other list is nullptr");
    }

    LinkedList<T> *result = new LinkedList<T>();
    
    Node *current_node = head;

    for (int i = 0; i < length; i++) {
        result->append(current_node->data);
        current_node = current_node->next;
    }

    current_node = other->head;

    for (int i = 0; i < other->length; i++) {
        result->append(current_node->data);
        current_node = current_node->next;
    }

    return result;
}

template <typename T>
LinkedList<T>::~LinkedList() {
    Node *current_node = head;

    while (current_node != nullptr) {
        Node* temp = current_node;
        current_node = current_node->next;
        delete temp;
    }
}
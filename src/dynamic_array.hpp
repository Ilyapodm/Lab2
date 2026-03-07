#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

template <typename T>
class DynamicArray {
public:
    DynamicArray(T* items, int count);
    DynamicArray(int size);
    DynamicArray(const DynamicArray<T> &dynamic_array);

    T get(int index);
    int get_size();

    void set(int index, T value);
    void resize(int new_size);
};

#endif  // DYNAMIC_ARRAY_HPP
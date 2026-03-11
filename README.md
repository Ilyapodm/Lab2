# 2 Lab O_o

# Methods details

## get_subsequence()
Результат get_subsequence() следует типу операнда (*this)
- immutable->get_subsequence() → Immutable
- mutable->get_subsequence()   → Mutable

## concat():
Результат concat следует типу левого операнда (*this)
- mutable->concat(immutable)   → Mutable    ("расширяем mutable")
- immutable->concat(mutable)   → Immutable  ("расширяем immutable")
- immutable->concat(immutable) → Immutable
- mutable->concat(mutable)     → Mutable

## IEnumerator<T>
Nested classes
IEnumerator<T> для ArraySequence реализован через сам ArraySequence.
IEnumerator<T> для ListSequence реализован через LinkedList (делегирование вниз). Так мы не тратим O(n) при использовании get

# Вопросы
- Где лучше определить итератор для массива: array sequence или dynamic array?
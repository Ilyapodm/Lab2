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

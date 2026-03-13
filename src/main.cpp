#include <iostream>
#include <stdexcept>
#include <climits>
#include <limits>
#include <string>

#include "array_sequence.hpp"
#include "list_sequence.hpp"
#include "bit_sequence.hpp"

// ═══════════════════════════════════════════════════════════════════════════
//  ШАГ 1: I/O утилиты
// ═══════════════════════════════════════════════════════════════════════════

static void flush_cin() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static int ask_int(const std::string& prompt) {
    int v;
    for (;;) {
        std::cout << prompt;
        if (std::cin >> v) { flush_cin(); return v; }
        std::cout << "  [!] Введите целое число\n";
        flush_cin();
    }
}

static int ask_range(const std::string& prompt, int lo, int hi) {
    for (;;) {
        int v = ask_int(prompt);
        if (v >= lo && v <= hi) return v;
        std::cout << "  [!] Введите от " << lo << " до " << hi << "\n";
    }
}

// ═══════════════════════════════════════════════════════════════════════════
//  ШАГ 2: Вывод последовательностей
// ═══════════════════════════════════════════════════════════════════════════

static void print_int_seq(const Sequence<int>* s, const std::string& label = "Sequence<int>") {
    std::cout << "\n  " << label << " [size=" << s->get_size() << "]: ";
    if (s->get_size() == 0) { std::cout << "(пусто)\n"; return; }
    for (int i = 0; i < s->get_size(); i++)
        std::cout << s->get(i) << (i + 1 < s->get_size() ? ", " : "\n");
}

static void print_bit_seq(const Sequence<Bit>* s, const std::string& label = "BitSequence") {
    std::cout << "\n  " << label << " [size=" << s->get_size() << "]: ";
    if (s->get_size() == 0) { std::cout << "(пусто)\n"; return; }
    for (int i = 0; i < s->get_size(); i++)
        std::cout << (s->get(i).get() ? '1' : '0');
    std::cout << "\n";
}

// ═══════════════════════════════════════════════════════════════════════════
//  ШАГ 3: Функторы для map / where / reduce
//  Используем обычные указатели на функции — лямбды здесь не подходят,
//  т.к. Sequence::map принимает T(*)(const T&), а не std::function
// ═══════════════════════════════════════════════════════════════════════════

// int map
static int fn_x2(const int& x)  { return x * 2; }
static int fn_sq(const int& x)  { return x * x; }
static int fn_neg(const int& x) { return -x; }
static int fn_inc(const int& x) { return x + 1; }

// int where
static bool fn_pos(const int& x)  { return x > 0; }
static bool fn_neg2(const int& x) { return x < 0; }
static bool fn_even(const int& x) { return x % 2 == 0; }
static bool fn_odd(const int& x)  { return x % 2 != 0; }

// int reduce
static int fn_sum(const int& a, const int& b)  { return a + b; }
static int fn_prod(const int& a, const int& b) { return a * b; }
static int fn_max(const int& a, const int& b)  { return a > b ? a : b; }
static int fn_min(const int& a, const int& b)  { return a < b ? a : b; }

// Bit map
static Bit fn_bnot(const Bit& b) { return ~b; }
static Bit fn_bid(const Bit& b)  { return b; }

// Bit where
static bool fn_is1(const Bit& b) { return  b.get(); }
static bool fn_is0(const Bit& b) { return !b.get(); }

// Bit reduce
static Bit fn_band(const Bit& a, const Bit& b) { return a & b; }
static Bit fn_bor(const Bit& a, const Bit& b)  { return a | b; }
static Bit fn_bxor(const Bit& a, const Bit& b) { return a ^ b; }

// ─── Меню выбора функторов ───────────────────────────────────────────────

static int (*pick_map_int())(const int&) {
    std::cout << "  Функция map:\n"
              << "    1. x*2   2. x*x   3. -x   4. x+1\n";
    switch (ask_range("  Выбор: ", 1, 4)) {
        case 1: return fn_x2;
        case 2: return fn_sq;
        case 3: return fn_neg;
        default: return fn_inc;
    }
}

static bool (*pick_where_int())(const int&) {
    std::cout << "  Условие where:\n"
              << "    1. x>0   2. x<0   3. чётное   4. нечётное\n";
    switch (ask_range("  Выбор: ", 1, 4)) {
        case 1: return fn_pos;
        case 2: return fn_neg2;
        case 3: return fn_even;
        default: return fn_odd;
    }
}

// ═══════════════════════════════════════════════════════════════════════════
//  ШАГ 4: Ввод временных последовательностей (для concat/битовых операций)
// ═══════════════════════════════════════════════════════════════════════════

// Всегда создаёт MutableArraySequence — concat принимает Sequence<T>*, тип не важен
static Sequence<int>* input_int_seq() {
    int n = ask_range("    Кол-во элементов: ", 0, 10000);
    auto* s = new MutableArraySequence<int>();
    for (int i = 0; i < n; i++)
        s->append(ask_int("    [" + std::to_string(i) + "]: "));
    return s;
}

// Строим через append, чтобы обойти баг в BitSequence(bool*, int):
// там отсутствует вызов ArraySequence<Bit>(size) → UB при size > 0
static MutableBitSequence* input_bit_seq() {
    int n = ask_range("    Кол-во бит: ", 0, 10000);
    auto* s = new MutableBitSequence();
    for (int i = 0; i < n; i++) {
        int b = ask_range("    бит[" + std::to_string(i) + "] (0/1): ", 0, 1);
        s->append(Bit(b != 0));  // MutableBitSequence::append → instance()=this
    }
    return s;
}

// ═══════════════════════════════════════════════════════════════════════════
//  ШАГ 5: Инициализация основной последовательности
//  Единый паттерн владения: if (next != s) { delete s; s = next; }
//  Работает и для Mutable (next == s), и для Immutable (next — новый объект)
// ═══════════════════════════════════════════════════════════════════════════

static Sequence<int>* init_int_seq(int type, bool is_mutable) {
    std::cout << "  Инициализация:\n"
              << "    1. Пустая\n"
              << "    2. Из ввода\n";
    int n = (ask_range("  Выбор: ", 1, 2) == 2)
            ? ask_range("  Кол-во элементов: ", 0, 10000)
            : 0;

    // Создаём конкретный тип — дальше работаем через Sequence<int>*
    Sequence<int>* s;
    if      (type == 1 && is_mutable)  s = new MutableArraySequence<int>();
    else if (type == 1 && !is_mutable) s = new ImmutableArraySequence<int>();
    else if (type == 2 && is_mutable)  s = new MutableListSequence<int>();
    else                               s = new ImmutableListSequence<int>();

    for (int i = 0; i < n; i++) {
        int v = ask_int("  [" + std::to_string(i) + "]: ");
        Sequence<int>* next = s->append(v);
        if (next != s) { delete s; s = next; }
    }
    return s;
}

static BitSequence* init_bit_seq(bool is_mutable) {
    std::cout << "  Инициализация:\n"
              << "    1. Пустая\n"
              << "    2. Из ввода\n";
    int n = (ask_range("  Выбор: ", 1, 2) == 2)
            ? ask_range("  Кол-во бит: ", 0, 10000)
            : 0;

    // Собираем через Sequence<Bit>*, потом приводим
    Sequence<Bit>* s = is_mutable
        ? (Sequence<Bit>*)new MutableBitSequence()
        : (Sequence<Bit>*)new ImmutableBitSequence();

    for (int i = 0; i < n; i++) {
        int b = ask_range("  бит[" + std::to_string(i) + "] (0/1): ", 0, 1);
        Sequence<Bit>* next = s->append(Bit(b != 0));
        if (next != s) { delete s; s = next; }
    }
    return static_cast<BitSequence*>(s);
}

// ═══════════════════════════════════════════════════════════════════════════
//  ШАГ 6: Основной цикл — Sequence<int>
// ═══════════════════════════════════════════════════════════════════════════

static void run_int(Sequence<int>* seq, bool is_mutable) {
    std::cout << "\n  ✓ Создана " << (is_mutable ? "Mutable" : "Immutable")
              << " Sequence<int>.";
    print_int_seq(seq);

    for (;;) {
        std::cout << "\n┌─ Sequence<int> ["
                  << (is_mutable ? "Mutable" : "Immutable") << "] ───────────────\n"
                  << "│  1. append           7. get_subsequence\n"
                  << "│  2. prepend          8. concat\n"
                  << "│  3. insert_at        9. map\n"
                  << "│  4. get(index)      10. where\n"
                  << "│  5. get_first/last  11. reduce\n"
                  << "│  6. get_size        12. Печать\n"
                  << "│  0. ← Назад\n"
                  << "└──────────────────────────────────────────\n";

        int cmd = ask_range("  Выбор: ", 0, 12);
        if (cmd == 0) break;

        try {
            switch (cmd) {
                case 1: {
                    int v = ask_int("  Значение: ");
                    Sequence<int>* res = seq->append(v);
                    if (res != seq) { delete seq; seq = res; }
                    print_int_seq(seq, "После append");
                    break;
                }
                case 2: {
                    int v = ask_int("  Значение: ");
                    Sequence<int>* res = seq->prepend(v);
                    if (res != seq) { delete seq; seq = res; }
                    print_int_seq(seq, "После prepend");
                    break;
                }
                case 3: {
                    std::cout << "  Размер: " << seq->get_size()
                              << ", допустимые индексы: 0.." << seq->get_size() << "\n";
                    int idx = ask_int("  Индекс: ");
                    int v   = ask_int("  Значение: ");
                    Sequence<int>* res = seq->insert_at(v, idx);
                    if (res != seq) { delete seq; seq = res; }
                    print_int_seq(seq, "После insert_at");
                    break;
                }
                case 4: {
                    int idx = ask_int("  Индекс: ");
                    std::cout << "  [" << idx << "] = " << seq->get(idx) << "\n";
                    break;
                }
                case 5: {
                    std::cout << "  get_first() = " << seq->get_first() << "\n"
                              << "  get_last()  = " << seq->get_last()  << "\n";
                    break;
                }
                case 6: {
                    std::cout << "  get_size() = " << seq->get_size() << "\n";
                    break;
                }
                case 7: {
                    std::cout << "  Размер: " << seq->get_size() << "\n";
                    int s = ask_int("  start: ");
                    int e = ask_int("  end:   ");
                    Sequence<int>* sub = seq->get_subsequence(s, e);
                    print_int_seq(sub, "Подпоследовательность");
                    delete sub;  // get_subsequence всегда новый объект
                    break;
                }
                case 8: {
                    std::cout << "  Введите 'other':\n";
                    Sequence<int>* other = input_int_seq();
                    Sequence<int>* res   = seq->concat(other);
                    print_int_seq(res, "Результат concat");
                    delete res;    // concat всегда новый объект
                    delete other;
                    break;
                }
                case 9: {
                    auto* fn = pick_map_int();
                    Sequence<int>* res = seq->map(fn);
                    if (res != seq) { delete seq; seq = res; }
                    print_int_seq(seq, "После map");
                    break;
                }
                case 10: {
                    auto* fn = pick_where_int();
                    Sequence<int>* res = seq->where(fn);
                    if (res != seq) { delete seq; seq = res; }
                    print_int_seq(seq, "После where");
                    break;
                }
                case 11: {
                    std::cout << "  Операция reduce:\n"
                              << "    1. Сумма      (нач=0)\n"
                              << "    2. Произведение (нач=1)\n"
                              << "    3. Максимум   (нач=INT_MIN)\n"
                              << "    4. Минимум    (нач=INT_MAX)\n";
                    int ch = ask_range("  Выбор: ", 1, 4);
                    int (*fns[])(const int&, const int&) = {fn_sum, fn_prod, fn_max, fn_min};
                    int starts[] = {0, 1, INT_MIN, INT_MAX};
                    const char* names[] = {"Сумма", "Произведение", "Максимум", "Минимум"};
                    std::cout << "  " << names[ch-1] << " = "
                              << seq->reduce(fns[ch-1], starts[ch-1]) << "\n";
                    break;
                }
                case 12: {
                    print_int_seq(seq);
                    break;
                }
            }
        }
        catch (const std::exception& e) {
            std::cout << "  [!] " << e.what() << "\n";
        }
    }

    delete seq;
}

// ═══════════════════════════════════════════════════════════════════════════
//  ШАГ 7: Основной цикл — BitSequence
//  bit_and/or/xor: результат усекается до min(size_a, size_b),
//  поэтому показываем оба размера перед операцией
// ═══════════════════════════════════════════════════════════════════════════

static void run_bits(BitSequence* seq, bool is_mutable) {
    std::cout << "\n  ✓ Создана " << (is_mutable ? "Mutable" : "Immutable")
              << " BitSequence.";
    print_bit_seq(seq);

    for (;;) {
        std::cout << "\n┌─ BitSequence ["
                  << (is_mutable ? "Mutable" : "Immutable") << "] ──────────────────\n"
                  << "│  1. append            9. bit_and (& с другой)\n"
                  << "│  2. prepend          10. bit_or  (| с другой)\n"
                  << "│  3. insert_at        11. bit_xor (^ с другой)\n"
                  << "│  4. get(index)       12. map\n"
                  << "│  5. get_first/last   13. where\n"
                  << "│  6. get_size         14. reduce\n"
                  << "│  7. get_subsequence  15. concat\n"
                  << "│  8. bit_not (~)      16. Печать\n"
                  << "│  0. ← Назад\n"
                  << "└──────────────────────────────────────────\n";

        int cmd = ask_range("  Выбор: ", 0, 16);
        if (cmd == 0) break;

        try {
            switch (cmd) {
                case 1: {
                    int b = ask_range("  Бит (0/1): ", 0, 1);
                    Sequence<Bit>* res = seq->append(Bit(b != 0));
                    if (res != seq) { delete seq; seq = static_cast<BitSequence*>(res); }
                    print_bit_seq(seq, "После append");
                    break;
                }
                case 2: {
                    int b = ask_range("  Бит (0/1): ", 0, 1);
                    Sequence<Bit>* res = seq->prepend(Bit(b != 0));
                    if (res != seq) { delete seq; seq = static_cast<BitSequence*>(res); }
                    print_bit_seq(seq, "После prepend");
                    break;
                }
                case 3: {
                    std::cout << "  Размер: " << seq->get_size()
                              << ", допустимые индексы: 0.." << seq->get_size() << "\n";
                    int idx = ask_int("  Индекс: ");
                    int b   = ask_range("  Бит (0/1): ", 0, 1);
                    Sequence<Bit>* res = seq->insert_at(Bit(b != 0), idx);
                    if (res != seq) { delete seq; seq = static_cast<BitSequence*>(res); }
                    print_bit_seq(seq, "После insert_at");
                    break;
                }
                case 4: {
                    int idx = ask_int("  Индекс: ");
                    std::cout << "  [" << idx << "] = "
                              << (seq->get(idx).get() ? 1 : 0) << "\n";
                    break;
                }
                case 5: {
                    std::cout << "  get_first() = " << (seq->get_first().get() ? 1 : 0) << "\n"
                              << "  get_last()  = " << (seq->get_last().get() ? 1 : 0)  << "\n";
                    break;
                }
                case 6: {
                    std::cout << "  get_size() = " << seq->get_size() << "\n";
                    break;
                }
                case 7: {
                    std::cout << "  Размер: " << seq->get_size() << "\n";
                    int s = ask_int("  start: ");
                    int e = ask_int("  end:   ");
                    Sequence<Bit>* sub = seq->get_subsequence(s, e);
                    print_bit_seq(sub, "Подпоследовательность");
                    delete sub;
                    break;
                }
                case 8: {
                    BitSequence* res = seq->bit_not();
                    if (res != seq) { delete seq; seq = res; }
                    print_bit_seq(seq, "После bit_not");
                    break;
                }
                case 9:
                case 10:
                case 11: {
                    const char* op_names[] = {"", "", "", "", "", "", "", "", "", "bit_and", "bit_or", "bit_xor"};
                    std::cout << "  Текущий размер: " << seq->get_size()
                              << "  (результат = min размеров)\n"
                              << "  Введите 'other':\n";
                    MutableBitSequence* other = input_bit_seq();
                    BitSequence* res = nullptr;
                    if (cmd == 9)  res = seq->bit_and(*other);
                    if (cmd == 10) res = seq->bit_or(*other);
                    if (cmd == 11) res = seq->bit_xor(*other);
                    delete other;
                    if (res != seq) { delete seq; seq = res; }
                    print_bit_seq(seq, std::string("После ") + op_names[cmd]);
                    break;
                }
                case 12: {
                    std::cout << "  Функция map:\n"
                              << "    1. NOT (инверсия)   2. Идентичность\n";
                    Bit (*fn)(const Bit&) = (ask_range("  Выбор: ", 1, 2) == 1)
                                           ? fn_bnot : fn_bid;
                    Sequence<Bit>* res = seq->map(fn);
                    if (res != seq) { delete seq; seq = static_cast<BitSequence*>(res); }
                    print_bit_seq(seq, "После map");
                    break;
                }
                case 13: {
                    std::cout << "  Условие where:\n"
                              << "    1. Оставить 1-биты   2. Оставить 0-биты\n";
                    bool (*fn)(const Bit&) = (ask_range("  Выбор: ", 1, 2) == 1)
                                            ? fn_is1 : fn_is0;
                    Sequence<Bit>* res = seq->where(fn);
                    if (res != seq) { delete seq; seq = static_cast<BitSequence*>(res); }
                    print_bit_seq(seq, "После where");
                    break;
                }
                case 14: {
                    std::cout << "  Операция reduce:\n"
                              << "    1. AND-all (нач=1): все ли биты = 1?\n"
                              << "    2. OR-all  (нач=0): есть ли хоть одна 1?\n"
                              << "    3. XOR-all (нач=0): чётность числа единиц\n";
                    int ch = ask_range("  Выбор: ", 1, 3);
                    Bit (*fns[])(const Bit&, const Bit&) = {fn_band, fn_bor, fn_bxor};
                    Bit starts[] = {Bit(true), Bit(false), Bit(false)};
                    const char* names[] = {"AND-all", "OR-all", "XOR-all"};
                    Bit result = seq->reduce(fns[ch-1], starts[ch-1]);
                    std::cout << "  " << names[ch-1] << " = " << (result.get() ? 1 : 0) << "\n";
                    break;
                }
                case 15: {
                    std::cout << "  Введите 'other' BitSequence:\n";
                    MutableBitSequence* other = input_bit_seq();
                    Sequence<Bit>* res = seq->concat(other);
                    print_bit_seq(res, "Результат concat");
                    delete res;
                    delete other;
                    break;
                }
                case 16: {
                    print_bit_seq(seq);
                    break;
                }
            }
        }
        catch (const std::exception& e) {
            std::cout << "  [!] " << e.what() << "\n";
        }
    }

    delete seq;
}

// ═══════════════════════════════════════════════════════════════════════════
//  ШАГ 8: Точка входа
// ═══════════════════════════════════════════════════════════════════════════

int main() {
    std::cout << "╔════════════════════════════════════════╗\n"
              << "║          Лабораторная работа 2         ║\n"
              << "╚════════════════════════════════════════╝\n";

    for (;;) {
        std::cout << "\n┌─ Главное меню ─────────────────────────\n"
                  << "│  Тип последовательности:\n"
                  << "│  1. ArraySequence<int>\n"
                  << "│  2. ListSequence<int>\n"
                  << "│  3. BitSequence\n"
                  << "│  0. Выход\n"
                  << "└────────────────────────────────────────\n";

        int type = ask_range("  Выбор: ", 0, 3);
        if (type == 0) break;

        std::cout << "  Изменяемость:\n"
                  << "    1. Mutable\n"
                  << "    2. Immutable\n";
        bool is_mutable = (ask_range("  Выбор: ", 1, 2) == 1);

        try {
            if (type == 3) {
                run_bits(init_bit_seq(is_mutable), is_mutable);
            } else {
                run_int(init_int_seq(type, is_mutable), is_mutable);
            }
        }
        catch (const std::exception& e) {
            std::cout << "  [!] " << e.what() << "\n";
        }
    }

    std::cout << "\n  До свидания!\n";
    return 0;
}

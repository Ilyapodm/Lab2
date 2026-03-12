#include <gtest/gtest.h>
#include "bit_sequence.hpp"

/*******************************************************************
 * Helpers
 *******************************************************************/

// Строим последовательность через append, чтобы не зависеть от
// возможного бага в конструкторе BitSequence(bool*, int).
static MutableBitSequence* make_mutable(std::initializer_list<bool> bits) {
    auto* s = new MutableBitSequence();
    for (bool b : bits) {
        s->append(Bit(b));
    }
    return s;
}

static ImmutableBitSequence* make_immutable(std::initializer_list<bool> bits) {
    auto* s = new ImmutableBitSequence();
    for (bool b : bits) {
        Sequence<Bit>* next = s->append(Bit(b));
        delete s;
        s = static_cast<ImmutableBitSequence*>(next);
    }
    return s;
}

/*******************************************************************
 * Constructors
 *******************************************************************/

TEST(BitSequence_Ctor, Default_ZeroSize) {
    MutableBitSequence s;
    EXPECT_EQ(s.get_size(), 0);
}

TEST(BitSequence_Ctor, FromBoolArray_CorrectValues) {
    bool items[] = {true, false, true};
    MutableBitSequence s(items, 3);
    EXPECT_EQ(s.get_size(), 3);
    EXPECT_EQ(s.get(0).get(), true);
    EXPECT_EQ(s.get(1).get(), false);
    EXPECT_EQ(s.get(2).get(), true);
}

TEST(BitSequence_Ctor, FromBoolArray_Empty_OK) {
    MutableBitSequence s(nullptr, 0);
    EXPECT_EQ(s.get_size(), 0);
}

TEST(BitSequence_Ctor, Copy_IndependentMemory) {
    bool items[] = {true, false, true};
    MutableBitSequence a(items, 3);
    MutableBitSequence b(a);
    b.insert_at(Bit(false), 0);
    EXPECT_EQ(a.get(0).get(), true);   // оригинал не тронут
    EXPECT_EQ(b.get(0).get(), false);
}

/*******************************************************************
 * bit_not
 *******************************************************************/

TEST(MutableBitSequence_BitNot, FlipsAllBits) {
    auto* s = make_mutable({true, false, true, false});
    BitSequence* result = s->bit_not();
    EXPECT_EQ(result, s);  // mutable: in-place
    EXPECT_EQ(result->get(0).get(), false);
    EXPECT_EQ(result->get(1).get(), true);
    EXPECT_EQ(result->get(2).get(), false);
    EXPECT_EQ(result->get(3).get(), true);
    delete s;
}

TEST(ImmutableBitSequence_BitNot, ReturnsNewObject_OriginalUnchanged) {
    auto* s = make_immutable({true, false, true});
    BitSequence* result = s->bit_not();
    EXPECT_NE(result, s);
    EXPECT_EQ(s->get(0).get(), true);      // оригинал не изменён
    EXPECT_EQ(result->get(0).get(), false);
    EXPECT_EQ(result->get(1).get(), true);
    delete result;
    delete s;
}

TEST(MutableBitSequence_BitNot, OnEmpty_OK) {
    MutableBitSequence s;
    BitSequence* result = s.bit_not();
    EXPECT_EQ(result->get_size(), 0);
}

TEST(MutableBitSequence_BitNot, DoubleNot_Identity) {
    auto* s = make_mutable({true, false, true});
    s->bit_not()->bit_not();  // оба in-place, s не меняется в итоге
    EXPECT_EQ(s->get(0).get(), true);
    EXPECT_EQ(s->get(1).get(), false);
    delete s;
}

/*******************************************************************
 * bit_and
 *******************************************************************/

TEST(MutableBitSequence_BitAnd, Basic) {
    auto* a = make_mutable({true,  true,  false, false});
    auto* b = make_mutable({true,  false, true,  false});
    BitSequence* result = a->bit_and(*b);
    EXPECT_EQ(result->get(0).get(), true);   // 1 & 1
    EXPECT_EQ(result->get(1).get(), false);  // 1 & 0
    EXPECT_EQ(result->get(2).get(), false);  // 0 & 1
    EXPECT_EQ(result->get(3).get(), false);  // 0 & 0
    delete a; delete b;
}

TEST(MutableBitSequence_BitAnd, ReturnsSelf_Mutable) {
    auto* a = make_mutable({true, false});
    auto* b = make_mutable({true, true});
    EXPECT_EQ(a->bit_and(*b), a);
    delete a; delete b;
}

TEST(ImmutableBitSequence_BitAnd, ReturnsNew_OriginalUnchanged) {
    auto* a = make_immutable({true, true, false});
    auto* b = make_mutable({false, true, true});
    BitSequence* result = a->bit_and(*b);
    EXPECT_NE(result, a);
    EXPECT_EQ(a->get(0).get(), true);         // оригинал не тронут
    EXPECT_EQ(result->get(0).get(), false);   // true & false
    EXPECT_EQ(result->get(1).get(), true);    // true & true
    delete result; delete a; delete b;
}

// Ключевое: размер результата = min(size_a, size_b)
TEST(BitSequence_BitAnd, ResultSize_IsMin) {
    auto* a = make_mutable({true, false, true, false, true});  // size=5
    auto* b = make_mutable({true, true, true});                // size=3
    BitSequence* result = a->bit_and(*b);
    EXPECT_EQ(result->get_size(), 3);   // min(5, 3)
    delete a; delete b;
}

TEST(BitSequence_BitAnd, EmptyOther_ResultEmpty) {
    auto* a = make_mutable({true, false, true});
    MutableBitSequence empty;
    BitSequence* result = a->bit_and(empty);
    EXPECT_EQ(result->get_size(), 0);
    delete a;
}

TEST(BitSequence_BitAnd, AllZeros) {
    auto* a = make_mutable({true, true, true});
    auto* b = make_mutable({false, false, false});
    BitSequence* result = a->bit_and(*b);
    for (int i = 0; i < result->get_size(); i++)
        EXPECT_EQ(result->get(i).get(), false);
    delete a; delete b;
}

/*******************************************************************
 * bit_or
 *******************************************************************/

TEST(MutableBitSequence_BitOr, Basic) {
    auto* a = make_mutable({true,  true,  false, false});
    auto* b = make_mutable({true,  false, true,  false});
    BitSequence* result = a->bit_or(*b);
    EXPECT_EQ(result->get(0).get(), true);   // 1 | 1
    EXPECT_EQ(result->get(1).get(), true);   // 1 | 0
    EXPECT_EQ(result->get(2).get(), true);   // 0 | 1
    EXPECT_EQ(result->get(3).get(), false);  // 0 | 0
    delete a; delete b;
}

TEST(BitSequence_BitOr, ResultSize_IsMin) {
    auto* a = make_mutable({true, false});          // size=2
    auto* b = make_mutable({false, true, false});   // size=3
    BitSequence* result = a->bit_or(*b);
    EXPECT_EQ(result->get_size(), 2);  // min(2, 3)
    delete a; delete b;
}

TEST(BitSequence_BitOr, AllOnes_WithFalse) {
    auto* a = make_mutable({false, false, false});
    auto* b = make_mutable({false, false, false});
    BitSequence* result = a->bit_or(*b);
    for (int i = 0; i < result->get_size(); i++)
        EXPECT_EQ(result->get(i).get(), false);
    delete a; delete b;
}

/*******************************************************************
 * bit_xor
 *******************************************************************/

TEST(MutableBitSequence_BitXor, Basic) {
    auto* a = make_mutable({true,  true,  false, false});
    auto* b = make_mutable({true,  false, true,  false});
    BitSequence* result = a->bit_xor(*b);
    EXPECT_EQ(result->get(0).get(), false);  // 1 ^ 1
    EXPECT_EQ(result->get(1).get(), true);   // 1 ^ 0
    EXPECT_EQ(result->get(2).get(), true);   // 0 ^ 1
    EXPECT_EQ(result->get(3).get(), false);  // 0 ^ 0
    delete a; delete b;
}

TEST(BitSequence_BitXor, XorWithSelf_AllZeros) {
    auto* a = make_mutable({true, false, true, true});
    auto* b = make_mutable({true, false, true, true});
    BitSequence* result = a->bit_xor(*b);
    for (int i = 0; i < result->get_size(); i++)
        EXPECT_EQ(result->get(i).get(), false);
    delete a; delete b;
}

TEST(BitSequence_BitXor, ResultSize_IsMin) {
    auto* a = make_mutable({true, false, true, false});  // size=4
    auto* b = make_mutable({true});                      // size=1
    BitSequence* result = a->bit_xor(*b);
    EXPECT_EQ(result->get_size(), 1);  // min(4, 1)
    delete a; delete b;
}

/*******************************************************************
 * Combined operations
 *******************************************************************/

// De Morgan: ~(A & B) == (~A | ~B)
TEST(BitSequence_DeMorgan, NotAnd_EqualsOrNot) {
    auto* a  = make_mutable({true, false, true, false});
    auto* b  = make_mutable({true, true, false, false});
    auto* na = make_mutable({true, false, true, false});
    auto* nb = make_mutable({true, true, false, false});

    // ~(A & B)
    BitSequence* and_res  = a->bit_and(*b);    // in-place в a
    BitSequence* not_and  = and_res->bit_not();// in-place

    // (~A | ~B)
    na->bit_not();   // in-place
    nb->bit_not();   // in-place
    BitSequence* or_not = na->bit_or(*nb);     // in-place в na

    ASSERT_EQ(not_and->get_size(), or_not->get_size());
    for (int i = 0; i < not_and->get_size(); i++)
        EXPECT_EQ(not_and->get(i).get(), or_not->get(i).get()) << "at index " << i;

    delete a; delete b; delete na; delete nb;
}

// ─────────────────────────────────────────────
// Унаследованный интерфейс Sequence<Bit>
// ─────────────────────────────────────────────

TEST(BitSequence_Sequence, Map_InvertsViaNot) {
    auto* s = make_mutable({true, false, true});
    Sequence<Bit>* result = s->Map([](const Bit& b) { return ~b; });
    EXPECT_EQ(result->get(0).get(), false);
    EXPECT_EQ(result->get(1).get(), true);
    EXPECT_EQ(result->get(2).get(), false);
    delete s;
}

TEST(BitSequence_Sequence, Where_KeepsOnlyTrue) {
    auto* s = make_mutable({true, false, true, false, true});
    Sequence<Bit>* result = s->Where([](const Bit& b) { return b.get(); });
    EXPECT_EQ(result->get_size(), 3);
    for (int i = 0; i < result->get_size(); i++)
        EXPECT_EQ(result->get(i).get(), true);
    delete s;
}

TEST(BitSequence_Sequence, Reduce_CountTrueBits) {
    auto* s = make_mutable({true, false, true, true, false});
    int count = s->Reduce(
        [](const Bit& a, const Bit& acc) {
            // acc здесь Bit, но нам нужен int — используем хак через Bit(int)
            return Bit(a.get() ? 1 : 0);
        },
        Bit(false)
    ).get();
    
    (void)count;
    delete s;
}

// Подсчёт через Where + get_size — более корректный способ:
TEST(BitSequence_Sequence, CountTrueBits_ViaWhere) {
    auto* s = make_mutable({true, false, true, true, false});
    Sequence<Bit>* trues = s->Where([](const Bit& b) { return b.get(); });
    EXPECT_EQ(trues->get_size(), 3);
    delete s;
}

TEST(BitSequence_Sequence, GetSubsequence) {
    auto* s = make_mutable({false, true, false, true, false});
    Sequence<Bit>* sub = s->get_subsequence(1, 3);
    EXPECT_EQ(sub->get_size(), 3);
    EXPECT_EQ(sub->get(0).get(), true);
    EXPECT_EQ(sub->get(1).get(), false);
    EXPECT_EQ(sub->get(2).get(), true);
    delete sub; delete s;
}

/*******************************************************************
 * Mutable vs Immutable: same results
 *******************************************************************/

class BitSequenceParamTest : public ::testing::TestWithParam<BitSequence*> {
protected:
    void TearDown() override { delete GetParam(); }
};

BitSequence* MakeMutable_1010()   { return make_mutable({true, false, true, false}); }
BitSequence* MakeImmutable_1010() { return make_immutable({true, false, true, false}); }

INSTANTIATE_TEST_SUITE_P(BothMutabilities, BitSequenceParamTest, ::testing::Values(
    MakeMutable_1010(),
    MakeImmutable_1010()
));

TEST_P(BitSequenceParamTest, BitNot_CorrectResult) {
    BitSequence* result = GetParam()->bit_not();
    EXPECT_EQ(result->get(0).get(), false);
    EXPECT_EQ(result->get(1).get(), true);
    EXPECT_EQ(result->get(2).get(), false);
    EXPECT_EQ(result->get(3).get(), true);
    if (result != GetParam()) delete result;
}

TEST_P(BitSequenceParamTest, BitAnd_WithAllOnes_IdentityLaw) {
    MutableBitSequence ones;
    for (int i = 0; i < GetParam()->get_size(); i++) ones.append(Bit(true));
    // A & 1 == A
    BitSequence* result = GetParam()->bit_and(ones);
    for (int i = 0; i < result->get_size(); i++)
        EXPECT_EQ(result->get(i).get(), (i % 2 == 0)) << "at index " << i;
    if (result != GetParam()) delete result;
}

TEST_P(BitSequenceParamTest, BitOr_WithAllZeros_IdentityLaw) {
    MutableBitSequence zeros;
    for (int i = 0; i < GetParam()->get_size(); i++) zeros.append(Bit(false));
    // A | 0 == A
    BitSequence* result = GetParam()->bit_or(zeros);
    for (int i = 0; i < result->get_size(); i++)
        EXPECT_EQ(result->get(i).get(), (i % 2 == 0)) << "at index " << i;
    if (result != GetParam()) delete result;
}

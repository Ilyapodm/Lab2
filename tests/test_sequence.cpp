#include <gtest/gtest.h>
#include "array_sequence.hpp"
#include "list_sequence.hpp"

// ─────────────────────────────────────────────
// Параметризованный fixture: Sequence<int>*
// ─────────────────────────────────────────────

class SequenceInterfaceTest : public ::testing::TestWithParam<Sequence<int>*> {
protected:
    void TearDown() override { delete GetParam(); }
};

// Fabrics for all methods
Sequence<int>* MakeEmpty_MutableArray()    { return new MutableArraySequence<int>(); }
Sequence<int>* MakeEmpty_ImmutableArray()  { return new ImmutableArraySequence<int>(); }
Sequence<int>* MakeEmpty_MutableList()     { return new MutableListSequence<int>(); }
Sequence<int>* MakeEmpty_ImmutableList()   { return new ImmutableListSequence<int>(); }

INSTANTIATE_TEST_SUITE_P(AllSequences, SequenceInterfaceTest, ::testing::Values(
    MakeEmpty_MutableArray(),
    MakeEmpty_ImmutableArray(),
    MakeEmpty_MutableList(),
    MakeEmpty_ImmutableList()
));

TEST_P(SequenceInterfaceTest, EmptySequence_ZeroSize) {
    EXPECT_EQ(GetParam()->get_size(), 0);
}

TEST_P(SequenceInterfaceTest, GetFirst_OnEmpty_Throws) {
    EXPECT_THROW(GetParam()->get_first(), std::out_of_range);
}

TEST_P(SequenceInterfaceTest, GetLast_OnEmpty_Throws) {
    EXPECT_THROW(GetParam()->get_last(), std::out_of_range);
}

TEST_P(SequenceInterfaceTest, Get_OnEmpty_Throws) {
    EXPECT_THROW(GetParam()->get(0), std::out_of_range);
}

TEST_P(SequenceInterfaceTest, Append_IncreasesSize) {
    Sequence<int>* s = GetParam()->append(42);
    EXPECT_EQ(s->get_size(), 1);
    EXPECT_EQ(s->get_first(), 42);
    EXPECT_EQ(s->get_last(),  42);
    if (s != GetParam()) delete s;
}

TEST_P(SequenceInterfaceTest, Prepend_IncreasesSize) {
    Sequence<int>* s = GetParam()->prepend(10);
    EXPECT_EQ(s->get_size(), 1);
    EXPECT_EQ(s->get_first(), 10);
    if (s != GetParam()) delete s;
}

TEST_P(SequenceInterfaceTest, AppendMultiple_OrderPreserved) {
    Sequence<int>* s = GetParam();
    for (int v : {1, 2, 3}) {
        Sequence<int>* next = s->append(v);
        if (next != s && s != GetParam()) delete s;
        s = next;
    }
    EXPECT_EQ(s->get_size(),  3);
    EXPECT_EQ(s->get(0),      1);
    EXPECT_EQ(s->get(2),      3);
    if (s != GetParam()) delete s;
}

/*******************************************************************
 * try_get
 *******************************************************************/

TEST_P(SequenceInterfaceTest, TryGet_OnEmpty_ReturnsNone) {
    auto opt = GetParam()->try_get(0);
    EXPECT_FALSE(opt.has_value());
}

TEST_P(SequenceInterfaceTest, TryGetFirst_OnEmpty_ReturnsNone) {
    EXPECT_FALSE(GetParam()->try_get_first().has_value());
}

TEST_P(SequenceInterfaceTest, TryGet_AfterAppend_ReturnsSome) {
    Sequence<int> *s = GetParam()->append(99);
    auto opt = s->try_get(0);
    EXPECT_TRUE(opt.has_value());
    EXPECT_EQ(opt.get_value(), 99);
    if (s != GetParam()) delete s;
}

/*******************************************************************
 * Map / Where / Reduce
 *******************************************************************/

TEST_P(SequenceInterfaceTest, Map_DoublesValues) {
    Sequence<int>* s = GetParam();
    for (int v : {1, 2, 3}) {
        Sequence<int>* next = s->append(v);
        if (next != s && s != GetParam()) delete s;
        s = next;
    }
    Sequence<int>* mapped = s->Map([](const int& x) { return x * 2; });
    EXPECT_EQ(mapped->get(0), 2);
    EXPECT_EQ(mapped->get(2), 6);
    if (mapped != s)    delete mapped;
    if (s != GetParam()) delete s;
}

TEST_P(SequenceInterfaceTest, Where_FiltersElements) {
    Sequence<int>* s = GetParam();
    for (int v : {1, 2, 3, 4}) {
        Sequence<int>* next = s->append(v);
        if (next != s && s != GetParam()) delete s;
        s = next;
    }
    Sequence<int>* filtered = s->Where([](const int& x) { return x % 2 == 0; });
    EXPECT_EQ(filtered->get_size(), 2);
    EXPECT_EQ(filtered->get(0), 2);
    EXPECT_EQ(filtered->get(1), 4);
    if (filtered != s)   delete filtered;
    if (s != GetParam()) delete s;
}

TEST_P(SequenceInterfaceTest, Reduce_Sum) {
    Sequence<int>* s = GetParam();
    for (int v : {1, 2, 3, 4, 5}) {
        Sequence<int>* next = s->append(v);
        if (next != s && s != GetParam()) delete s;
        s = next;
    }
    int sum = s->Reduce([](const int& a, const int& b) { return a + b; }, 0);
    EXPECT_EQ(sum, 15);
    if (s != GetParam()) delete s;
}

/*******************************************************************
 * concat / get_subsequence
 *******************************************************************/

TEST_P(SequenceInterfaceTest, Concat_JoinsTwo) {
    int d[] = {1, 2, 3};
    Sequence<int>* other = new MutableArraySequence<int>(d, 3);
    Sequence<int>* s = GetParam();
    for (int v : {10, 20}) {
        Sequence<int>* next = s->append(v);
        if (next != s && s != GetParam()) delete s;
        s = next;
    }
    Sequence<int>* result = s->concat(other);
    EXPECT_EQ(result->get_size(), 5);
    EXPECT_EQ(result->get(0), 10);
    EXPECT_EQ(result->get(4), 3);
    delete result;
    delete other;
    if (s != GetParam()) delete s;
}
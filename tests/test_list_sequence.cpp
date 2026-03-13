#include <gtest/gtest.h>
#include "list_sequence.hpp"
#include "array_sequence.hpp"  // for some tests 

/*******************************************************************
 * Mutable
 *******************************************************************/

TEST(MutableListSequence, Append_ReturnsSelf) {
    MutableListSequence<int> s;
    Sequence<int>* ret = s.append(1);
    EXPECT_EQ(ret, &s);
}

TEST(MutableListSequence, Prepend_ReturnsSelf) {
    MutableListSequence<int> s;
    Sequence<int>* ret = s.prepend(99);
    EXPECT_EQ(ret, &s);
}

TEST(MutableListSequence, InsertAt_MutatesInPlace) {
    int d[] = {1, 3};
    MutableListSequence<int> s(d, 2);
    Sequence<int>* ret = s.insert_at(2, 1);
    EXPECT_EQ(ret, &s);
    EXPECT_EQ(s.get(1), 2);
    EXPECT_EQ(s.get_size(), 3);
}

TEST(MutableListSequence, map_MutatesInPlace) {
    int d[] = {1, 2, 3};
    MutableListSequence<int> s(d, 3);
    Sequence<int>* ret = s.map([](const int& x) { return x * 3; });
    EXPECT_EQ(ret, &s);
    EXPECT_EQ(s.get(0), 3);
    EXPECT_EQ(s.get(2), 9);
}

TEST(MutableListSequence, where_MutatesInPlace) {
    int d[] = {10, 11, 12, 13, 14};
    MutableListSequence<int> s(d, 5);
    Sequence<int>* ret = s.where([](const int& x) { return x % 2 == 0; });
    EXPECT_EQ(ret, &s);
    EXPECT_EQ(s.get_size(), 3);
    EXPECT_EQ(s.get(0), 10);
    EXPECT_EQ(s.get(2), 14);
}

/*******************************************************************
 * Immutable
 *******************************************************************/

TEST(ImmutableListSequence, Append_ReturnsNewObject) {
    ImmutableListSequence<int> s;
    Sequence<int>* ret = s.append(7);
    EXPECT_NE(ret, &s);
    EXPECT_EQ(s.get_size(), 0);
    EXPECT_EQ(ret->get_size(), 1);
    EXPECT_EQ(ret->get(0), 7);
    delete ret;
}

TEST(ImmutableListSequence, Prepend_OriginalUnchanged) {
    int d[] = {2, 3};
    ImmutableListSequence<int> s(d, 2);
    Sequence<int>* ret = s.prepend(1);
    EXPECT_EQ(s.get_size(), 2);
    EXPECT_EQ(ret->get(0), 1);
    EXPECT_EQ(ret->get_size(), 3);
    delete ret;
}

TEST(ImmutableListSequence, map_OriginalUnchanged) {
    int d[] = {2, 4, 6};
    ImmutableListSequence<int> s(d, 3);
    Sequence<int>* ret = s.map([](const int& x) { return x / 2; });
    EXPECT_EQ(s.get(0), 2);
    EXPECT_EQ(ret->get(0), 1);
    delete ret;
}

TEST(ImmutableListSequence, where_OriginalUnchanged) {
    int d[] = {1, 2, 3, 4};
    ImmutableListSequence<int> s(d, 4);
    Sequence<int>* ret = s.where([](const int& x) { return x > 2; });
    EXPECT_EQ(s.get_size(), 4);
    EXPECT_EQ(ret->get_size(), 2);
    delete ret;
}

/*******************************************************************
 * get_subsequence - always new object
 *******************************************************************/

TEST(ListSequence_Subsequence, CorrectValues) {
    int d[] = {10, 20, 30, 40, 50};
    MutableListSequence<int> s(d, 5);
    Sequence<int>* sub = s.get_subsequence(1, 3);
    EXPECT_EQ(sub->get_size(), 3);
    EXPECT_EQ(sub->get(0), 20);
    EXPECT_EQ(sub->get(2), 40);
    delete sub;
}

TEST(ListSequence_Subsequence, SingleElement) {
    int d[] = {5, 10, 15};
    MutableListSequence<int> s(d, 3);
    Sequence<int>* sub = s.get_subsequence(0, 0);
    EXPECT_EQ(sub->get_size(), 1);
    EXPECT_EQ(sub->get(0), 5);
    delete sub;
}

TEST(ListSequence_Subsequence, InvalidRange_Throws) {
    int d[] = {1, 2, 3};
    MutableListSequence<int> s(d, 3);
    EXPECT_THROW(s.get_subsequence(2, 1),  std::out_of_range);
    EXPECT_THROW(s.get_subsequence(-1, 1), std::out_of_range);
    EXPECT_THROW(s.get_subsequence(0, 3),  std::out_of_range);
}

/*******************************************************************
 * Insert_at
 *******************************************************************/

// TEST(MutableArraySequence, InsertAt_AtZero_EqualsPrepend) {
//     int d[] = {2, 3, 4};
//     MutableListSequence<int> s(d, 3);
//     s.insert_at(1, 0);
//     EXPECT_EQ(s.get_first(), 1);
//     EXPECT_EQ(s.get_size(), 4);
// }

// TEST(MutableArraySequence, InsertAt_AtSize_EqualsAppend) {
//     int d[] = {1, 2, 3};
//     MutableListSequence<int> s(d, 3);
//     s.insert_at(4, 3);
//     EXPECT_EQ(s.get_last(), 4);
// }

// TEST(MutableArraySequence, InsertAt_BeyondSize_Throws) {
//     MutableListSequence<int> s;
//     EXPECT_THROW(s.insert_at(1, 1), std::out_of_range);
// }

/*******************************************************************
 * concat
 *******************************************************************/

TEST(ListSequence_Concat, ResultIsNew_OriginalUnchanged) {
    int d1[] = {1, 2}, d2[] = {3, 4};
    MutableListSequence<int> a(d1, 2);
    MutableListSequence<int> b(d2, 2);
    Sequence<int>* res = a.concat(&b);
    EXPECT_NE(res, &a);
    EXPECT_EQ(a.get_size(), 2);
    EXPECT_EQ(res->get_size(), 4);
    EXPECT_EQ(res->get(0), 1);
    EXPECT_EQ(res->get(3), 4);
    delete res;
}

TEST(ListSequence_Concat, CrossType_ListWithArray) {
    // concat принимает Sequence* — можно передать ArraySequence
    int d1[] = {1, 2};
    int d2[] = {3, 4};
    MutableListSequence<int> list(d1, 2);
    MutableArraySequence<int> arr(d2, 2);
    Sequence<int>* res = list.concat(&arr);
    EXPECT_EQ(res->get_size(), 4);
    EXPECT_EQ(res->get(2), 3);
    delete res;
}

/*******************************************************************
 * Enumerator
 *******************************************************************/

TEST(MutableListSequence, Enumerator_IteratesAll) {
    int d[] = {1, 2, 3, 4, 5};
    MutableListSequence<int> s(d, 5);
    IEnumerator<int>* it = s.get_enumerator();
    int sum = 0;
    while (it->move_next()) sum += it->get_current();
    delete it;
    EXPECT_EQ(sum, 15);
}

TEST(ImmutableListSequence, Enumerator_OnEmpty_MoveNextFalse) {
    ImmutableListSequence<int> s;
    IEnumerator<int>* it = s.get_enumerator();
    EXPECT_FALSE(it->move_next());
    delete it;
}

/*******************************************************************
 * operator=
 *******************************************************************/

TEST(MutableListSequence, AssignOperator_DeepCopy) {
    int d[] = {1, 2, 3};
    MutableListSequence<int> a(d, 3);
    MutableListSequence<int> b;
    b = a;
    // изменяем b — a не должна измениться
    b.append(4);
    EXPECT_EQ(a.get_size(), 3);
    EXPECT_EQ(b.get_size(), 4);
}

TEST(ImmutableListSequence, AssignOperator_SelfAssign) {
    int d[] = {10, 20};
    ImmutableListSequence<int> s(d, 2);
    s = s;
    EXPECT_EQ(s.get_size(), 2);
    EXPECT_EQ(s.get(0), 10);
}
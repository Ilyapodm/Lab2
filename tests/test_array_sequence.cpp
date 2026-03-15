#include <gtest/gtest.h>
#include "array_sequence.hpp"
//BUG не написано slice, set, remove_at
/*******************************************************************
 * Mutable
 *******************************************************************/

TEST(MutableArraySequence, Append_ReturnsSelf) {
    MutableArraySequence<int> s;
    Sequence<int>* ret = s.append(1);
    EXPECT_EQ(ret, &s);  // the object, not copy
}

TEST(MutableArraySequence, Prepend_ReturnsSelf) {
    MutableArraySequence<int> s;
    Sequence<int>* ret = s.prepend(1);
    EXPECT_EQ(ret, &s);
}

TEST(MutableArraySequence, InsertAt_ReturnsSelf) {
    MutableArraySequence<int> s;
    s.append(1); s.append(3);
    Sequence<int>* ret = s.insert_at(2, 1);
    EXPECT_EQ(ret, &s);
    EXPECT_EQ(s.get(1), 2);
}

TEST(MutableArraySequence, map_MutatesInPlace) {
    int d[] = {1, 2, 3};
    MutableArraySequence<int> s(d, 3);
    Sequence<int>* ret = s.map([](const int& x) { return x * 10; });
    EXPECT_EQ(ret, &s);
    EXPECT_EQ(s.get(0), 10);
    EXPECT_EQ(s.get(2), 30);
}

TEST(MutableArraySequence, where_MutatesInPlace) {
    int d[] = {1, 2, 3, 4, 5};
    MutableArraySequence<int> s(d, 5);
    Sequence<int>* ret = s.where([](const int& x) { return x > 3; });
    EXPECT_EQ(ret, &s);
    EXPECT_EQ(s.get_size(), 2);
    EXPECT_EQ(s.get(0), 4);
    EXPECT_EQ(s.get(1), 5);
}

TEST(MutableArraySequence, slice_MutatesInPlace) {
    int d1[] = {1, 2, 3, 4, 5};
    int d2[] = {99, 88};
    MutableArraySequence<int> s(d1, 5);
    MutableArraySequence<int> a(d2, 2);
    Sequence<int>* ret = s.slice(1, 2, a);
    EXPECT_EQ(ret, &s);
    EXPECT_EQ(s.get_size(), 5);
    EXPECT_EQ(s.get(0), 1);
    EXPECT_EQ(s.get(1), 99);
}
/*******************************************************************
 * Immutable
 *******************************************************************/

TEST(ImmutableArraySequence, Append_ReturnsNewObject) {
    ImmutableArraySequence<int> s;
    Sequence<int>* ret = s.append(42);
    EXPECT_NE(ret, &s);         // new object
    EXPECT_EQ(s.get_size(), 0);  // original doesn't change
    EXPECT_EQ(ret->get_size(), 1);
    EXPECT_EQ(ret->get(0), 42);
    delete ret;
}

TEST(ImmutableArraySequence, ChainedAppend_EachStepNewObject) {
    ImmutableArraySequence<int> s;
    Sequence<int>* s1 = s.append(1);
    Sequence<int>* s2 = s1->append(2);
    EXPECT_NE(s1, s2);
    EXPECT_EQ(s.get_size(),  0);
    EXPECT_EQ(s1->get_size(), 1);
    EXPECT_EQ(s2->get_size(), 2);
    delete s1;
    delete s2;
}

TEST(ImmutableArraySequence, Prepend_OriginalUnchanged) {
    int d[] = {2, 3};
    ImmutableArraySequence<int> s(d, 2);
    Sequence<int>* ret = s.prepend(1);
    EXPECT_EQ(s.get_size(), 2);   // original doesn't change
    EXPECT_EQ(ret->get_size(), 3);
    EXPECT_EQ(ret->get(0), 1);
    delete ret;
}

TEST(ImmutableArraySequence, map_OriginalUnchanged) {
    int d[] = {1, 2, 3};
    ImmutableArraySequence<int> s(d, 3);
    Sequence<int>* ret = s.map([](const int& x) { return x * 2; });
    EXPECT_EQ(s.get(0), 1);      // original doesn't change
    EXPECT_EQ(ret->get(0), 2);
    delete ret;
}

TEST(ImmutableArraySequence, where_OriginalUnchanged) {
    int d[] = {1, 2, 3, 4};
    ImmutableArraySequence<int> s(d, 4);
    Sequence<int>* ret = s.where([](const int& x) { return x % 2 == 0; });
    EXPECT_EQ(s.get_size(), 4);
    EXPECT_EQ(ret->get_size(), 2);
    delete ret;
}

// TEST(ImmutableArraySequence, where_OriginalUnchanged) {
//     int d[] = {1, 2, 3, 4};
//     ImmutableArraySequence<int> s(d, 4);
//     Sequence<int>* ret = s.where([](const int& x) { return x % 2 == 0; });
//     EXPECT_EQ(s.get_size(), 4);
//     EXPECT_EQ(ret->get_size(), 2);
//     delete ret;
// }

/*******************************************************************
 * get_subsequence - always new object
 *******************************************************************/

TEST(ArraySequence_Subsequence, MutableReturnsNew) {
    int d[] = {10, 20, 30, 40, 50};
    MutableArraySequence<int> s(d, 5);
    Sequence<int>* sub = s.get_subsequence(1, 3);
    EXPECT_NE(sub, &s);
    EXPECT_EQ(sub->get_size(), 3);
    EXPECT_EQ(sub->get(0), 20);
    EXPECT_EQ(sub->get(2), 40);
    delete sub;
}

TEST(ArraySequence_Subsequence, SingleElement) {
    int d[] = {10, 20, 30};
    MutableArraySequence<int> s(d, 3);
    Sequence<int>* sub = s.get_subsequence(2, 2);
    EXPECT_EQ(sub->get_size(), 1);
    EXPECT_EQ(sub->get(0), 30);
    delete sub;
}

TEST(ArraySequence_Subsequence, InvalidRange_Throws) {
    int d[] = {1, 2, 3};
    MutableArraySequence<int> s(d, 3);
    EXPECT_THROW(s.get_subsequence(2, 1),  std::out_of_range);
    EXPECT_THROW(s.get_subsequence(-1, 1), std::out_of_range);
    EXPECT_THROW(s.get_subsequence(0, 3),  std::out_of_range);
}

/*******************************************************************
 * Insert_at
 *******************************************************************/

TEST(MutableArraySequence, InsertAt_AtZero_EqualsPrepend) {
    int d[] = {2, 3, 4};
    MutableArraySequence<int> s(d, 3);
    s.insert_at(1, 0);
    EXPECT_EQ(s.get_first(), 1);
    EXPECT_EQ(s.get_size(), 4);
}

TEST(MutableArraySequence, InsertAt_AtSize_EqualsAppend) {
    int d[] = {1, 2, 3};
    MutableArraySequence<int> s(d, 3);
    s.insert_at(4, 3);
    EXPECT_EQ(s.get_last(), 4);
}

TEST(MutableArraySequence, InsertAt_BeyondSize_Throws) {
    MutableArraySequence<int> s;
    EXPECT_THROW(s.insert_at(1, 1), std::out_of_range);
}

/*******************************************************************
 * concat
 *******************************************************************/

TEST(ArraySequence_Concat, ResultIsNew_OriginalUnchanged) {
    int d1[] = {1, 2}, d2[] = {3, 4, 5};
    MutableArraySequence<int> a(d1, 2);
    MutableArraySequence<int> b(d2, 3);
    Sequence<int>* res = a.concat(b);
    EXPECT_NE(res, &a);
    EXPECT_EQ(a.get_size(), 2);  // original doesn't change
    EXPECT_EQ(res->get_size(), 5);
    EXPECT_EQ(res->get(0), 1);
    EXPECT_EQ(res->get(4), 5);
    delete res;
}

TEST(ArraySequence_Concat, WithEmpty) {
    int d[] = {1, 2, 3};
    MutableArraySequence<int> a(d, 3);
    MutableArraySequence<int> empty;
    Sequence<int>* res = a.concat(empty);
    EXPECT_EQ(res->get_size(), 3);
    delete res;
}

/*******************************************************************
 * Enumerator
 *******************************************************************/

TEST(MutableArraySequence, Enumerator_IteratesAll) {
    int d[] = {5, 10, 15};
    MutableArraySequence<int> s(d, 3);
    IEnumerator<int>* it = s.get_enumerator();
    int sum = 0;
    while (it->move_next()) sum += it->get_current();
    delete it;
    EXPECT_EQ(sum, 30);
}

TEST(MutableArraySequence, Enumerator_Reset_ReIterates) {
    int d[] = {1, 2, 3};
    MutableArraySequence<int> s(d, 3);
    IEnumerator<int>* it = s.get_enumerator();
    while (it->move_next()) {}
    it->reset();
    int count = 0;
    while (it->move_next()) count++;
    delete it;
    EXPECT_EQ(count, 3);
}

/*******************************************************************
 * operator=
 *******************************************************************/

TEST(MutableArraySequence, AssignOperator_DeepCopy) {
    int d[] = {1, 2, 3};
    MutableArraySequence<int> a(d, 3);
    MutableArraySequence<int> b;
    b = a;
    // изменяем b — a не должна измениться
    b.append(4);
    EXPECT_EQ(a.get_size(), 3);
    EXPECT_EQ(b.get_size(), 4);
}

TEST(ImmutableArraySequence, AssignOperator_SelfAssign) {
    int d[] = {10, 20};
    ImmutableArraySequence<int> s(d, 2);
    s = s;
    EXPECT_EQ(s.get_size(), 2);
    EXPECT_EQ(s.get(0), 10);
}
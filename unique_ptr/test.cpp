#include <gtest/gtest.h>

#include "unique_ptr.h"

TEST(UniquePtr, Constructors) { // NOLINT
{ // default constructor
UniquePtr<int> a;
ASSERT_FALSE(a);
ASSERT_EQ(a.Get(), nullptr);
}

{ // pointer constructor
UniquePtr<int> a(nullptr);
ASSERT_FALSE(a);
ASSERT_EQ(a.Get(), nullptr);

auto ptr = new int;
UniquePtr<int> b(ptr);
ASSERT_TRUE(b);
ASSERT_EQ(b.Get(), ptr);
}

{ // move constructor
auto ptr = new int;
UniquePtr<int> a(ptr);
UniquePtr<int> b(std::move(a));
ASSERT_TRUE(b);
ASSERT_FALSE(a); // NOLINT check moved in valid state
ASSERT_EQ(a.Get(), nullptr);
ASSERT_EQ(b.Get(), ptr);
// make sure this causes CE
// UniquePtr<int> c(b);
}
}

TEST(UniquePtr, Assignment) { // NOLINT
auto ptr = new int;
UniquePtr<int> a(ptr);

{
UniquePtr<int> b = std::move(a);
ASSERT_TRUE(b);
ASSERT_FALSE(a); // NOLINT check moved in valid state
ASSERT_EQ(a.Get(), nullptr);
ASSERT_EQ(b.Get(), ptr);
a = std::move(b);
}
ASSERT_TRUE(a);
ASSERT_EQ(a.Get(), ptr);
}

TEST(UniquePtr, Release) { // NOLINT
{ // Empty
UniquePtr<int> a;
ASSERT_EQ(a.Release(), nullptr);
}

{ // With content
auto ptr = new int;
UniquePtr<int> a(ptr);
ASSERT_EQ(a.Release(), ptr);
ASSERT_EQ(a.Get(), nullptr);
ASSERT_FALSE(a);
delete ptr;
}
}

TEST(UniquePtr, Reset) { // NOLINT
{ // Empty
auto ptr = new int;
UniquePtr<int> a;
a.Reset(ptr);
ASSERT_TRUE(a);
ASSERT_EQ(a.Get(), ptr);
}

{ // With content
auto ptr1 = new int;
auto ptr2 = new int;
UniquePtr<int> a(ptr1);
a.Reset(ptr2);
ASSERT_TRUE(a);
ASSERT_EQ(a.Get(), ptr2);
}
}

struct A {
    int x;
    double y;

    explicit A(int x = 0, double y = 0.0) : x(x), y(y) {
    }
};

TEST(UniquePtr, Operators) { // NOLINT
UniquePtr<A> a(new A);
ASSERT_EQ(a->x, 0);
ASSERT_EQ((*a).x, 0.0);
}

#ifdef MAKE_UNIQUE_IMPLEMENTED

TEST(UniquePtr, MakeUnique) { // NOLINT
    {
        UniquePtr<A> ptr = MakeUnique<A>();
        ASSERT_EQ(ptr->x, 0);
        ASSERT_EQ(ptr->y, 0.0);
    }

    {
        UniquePtr<A> ptr = MakeUnique<A>(11);
        ASSERT_EQ(ptr->x, 11);
        ASSERT_EQ(ptr->y, 0.0);
    }

    {
        UniquePtr<A> ptr = MakeUnique<A>(11, 0.5);
        ASSERT_EQ(ptr->x, 11);
        ASSERT_EQ(ptr->y, 0.5);
    }
}

#endif

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


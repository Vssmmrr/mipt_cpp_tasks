#include <gtest/gtest.h>

#include "any.h"

TEST(Any, HasConstructors) { // NOLINT
    Any a; // no CE check

    Any b(11);
    ASSERT_EQ(any_cast<int>(b), 11);

    Any c(b);
    ASSERT_EQ(any_cast<int>(b), 11);
    ASSERT_EQ(any_cast<int>(c), 11);

    Any d(std::move(c));
    ASSERT_EQ(any_cast<int>(d), 11);
}

TEST(Any, Assignment) { // NOLINT
    Any a;
    Any b;

    { // value assignment
        a = 11;
        ASSERT_EQ(any_cast<int>(a), 11);
    }

    { // reassigning
        a = &a;
        ASSERT_EQ(any_cast<Any*>(a), &a);
    }

    { // copy assignment
        b = a;
        ASSERT_EQ(any_cast<Any*>(b), &a);
    }

    { // copy is independent
        b = 1.0;
        ASSERT_EQ(any_cast<Any*>(a), &a);
        ASSERT_EQ(any_cast<double>(b), 1.0);
    }

    { // move
        a = std::move(b);
        ASSERT_EQ(any_cast<double>(a), 1.0);
    }

    { // self-assignment
        a = a;
        ASSERT_EQ(any_cast<double>(a), 1.0);
    }
}

TEST(Any, Swap) { // NOLINT
    Any a = 1;
    Any b = &a;

    a.Swap(b);
    ASSERT_EQ(any_cast<Any*>(a), &a);
    ASSERT_EQ(any_cast<int>(b), 1);

    b.Swap(b);
    ASSERT_EQ(any_cast<int>(b), 1);
}

TEST(Any, HasValue) { // NOLINT
    Any a;
    ASSERT_FALSE(a.HasValue());

    Any b = 11;
    ASSERT_TRUE(b.HasValue());

    a = b;
    ASSERT_TRUE(a.HasValue());
    ASSERT_TRUE(b.HasValue());

    a = std::move(b);
    ASSERT_TRUE(a.HasValue());
    ASSERT_FALSE(b.HasValue()); // NOLINT check moved valid state

    a.Reset();
    ASSERT_FALSE(a.HasValue());
}

TEST(Any, BadAnyCast) { // NOLINT
    Any a;
    ASSERT_THROW(any_cast<int>(a), BadAnyCast);

    a = 11;
    ASSERT_THROW(any_cast<char*>(a), BadAnyCast);
}

#include "cctest/cctest.h"

namespace {

FIXTURE(ValueInitSpec1) {
    struct Foo {
        int a;
        int b;
    };

    TEST("value init 1: for trivial default cons") {
        Foo foo {};
        ASSERT_EQ(0, foo.a);
        ASSERT_EQ(0, foo.b);        
    }

    TEST("value init 2: for trivial default cons") {
        Foo foo = {};
        ASSERT_EQ(0, foo.a);
        ASSERT_EQ(0, foo.b);        
    }

    TEST("value init 3: for trivial default cons") {
        auto foo = Foo {};
        ASSERT_EQ(0, foo.a);
        ASSERT_EQ(0, foo.b);        
    }

    TEST("value init 4: for trivial default cons") {
        auto foo = Foo();
        ASSERT_EQ(0, foo.a);
        ASSERT_EQ(0, foo.b);        
    }

    TEST("non initialized for trivial default cons") {
        Foo foo;
        ASSERT_NE(0, foo.a);
        ASSERT_NE(0, foo.b);        
    }
};

} // namespace
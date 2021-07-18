#include "cctest/cctest.h"
#include "ccspec/base/type_assert.h"

namespace {

FIXTURE(LvalueSpec) {
    int i = 10;

    TEST("lvalue: named variable is lvalue") {
        auto &&result = i; 
        STATIC_ASSERT_LVALUE_REF(result);
    }

    TEST("lvalue: assignment expr that return ref type") {
        auto &&result = (i=10);
        STATIC_ASSERT_LVALUE_REF(result);

        auto &&sum = (i+=10);
        STATIC_ASSERT_LVALUE_REF(sum);
    }

    TEST("lvalue: subscript expr that return ref type") {
        int arr[] = {1, 2, 3};
        auto &&element = arr[2];
        STATIC_ASSERT_LVALUE_REF(element);
    }

    TEST("lvalue: prefix increment or decrement expr return noref type") {
        auto &&prefix = ++i;
        STATIC_ASSERT_LVALUE_REF(prefix);
    }

    TEST("lvalue: prefix increment or decrement expr return noref type") {
        auto &&prefix = ++i;
        STATIC_ASSERT_LVALUE_REF(prefix);
    }

    TEST("lvalue: static_cast<T&>") {
        auto &&prefix = static_cast<int&>(i);
        STATIC_ASSERT_LVALUE_REF(prefix);
    }
};

} // namespace
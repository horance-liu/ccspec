#include "cctest/cctest.h"
#include "ccspec/base/type_assert.h"

namespace {

FIXTURE(RvalueSpec) {

    TEST("rvalue: literal_constant") {
        auto &&i = 10;
        STATIC_ASSERT_RVALUE_REF(i);

        auto &&b = true;
        STATIC_ASSERT_RVALUE_REF(b);

        auto &&d = 0.0;
        STATIC_ASSERT_RVALUE_REF(d);

        auto &&f = 0.0f;
        STATIC_ASSERT_RVALUE_REF(f);

        auto &&c = 'c';
        STATIC_ASSERT_RVALUE_REF(c);

        auto &&l = {1, 2, 3};
        STATIC_ASSERT_RVALUE_REF(l);
    }

    TEST("rvalue: c_style_string_is_not_rvalue") {
        auto &&s = "c++11";
        STATIC_ASSERT_TYPE(const char (&)[6], s);
        STATIC_ASSERT_LVALUE_REF(s);
    }

    int i = 10;

    TEST("rvalue: arithmetic_expr_return_noref_type") {
        auto &&sum = i + 10;
        STATIC_ASSERT_RVALUE_REF(sum);
    }

    TEST("rvalue: relation_expr_return_noref_type") {
        auto &&less = i < 10;
        STATIC_ASSERT_RVALUE_REF(less);
    }

    TEST("rvalue: postfix_increment_or_decrement_expr_return_noref_type") {
        auto &&postfix = i++;
        STATIC_ASSERT_RVALUE_REF(postfix);
    }

    TEST("rvalue: bitwise_expr_return_noref_type") {
        auto &&bitwise = i & 0xFF;
        STATIC_ASSERT_RVALUE_REF(bitwise);
    }

    TEST("rvalue: conversion_expr_return_noref_type") {
        auto &&conversion = static_cast<short>(i);
        STATIC_ASSERT_RVALUE_REF(conversion);
    }

    TEST("rvalue: lambda_expr") {
        auto &&lambda = [](){ return false; };
        STATIC_ASSERT_RVALUE_REF(lambda);
    }

    TEST("rvalue: std_move") {
        auto &&rvalue = std::move(i);
        STATIC_ASSERT_RVALUE_REF(rvalue);
    }

    TEST("rvalue: functions_that_return_noref_type") {
        auto &&ptr = std::make_shared<int>(10);
        STATIC_ASSERT_TYPE(std::shared_ptr<int>&&, ptr);
        STATIC_ASSERT_RVALUE_REF(ptr);
    }


    struct Foo
    {
        Foo() {};

        const std::string& qualifier() const &
        {
            return m_qualifier;
        }

        std::string& qualifier() &
        {
            return m_qualifier = "lvalue ref qualifier";
        }

        std::string&& qualifier() &&
        {
            return std::move(m_qualifier = "rvalue ref qualifier");
        }

    private:
        std::string m_qualifier = "const lvalue ref qualifier";
    };


    TEST("rvalue: const_lvalue_ref_qualifier") {
        const Foo cfoo;
        ASSERT_EQ(std::string("const lvalue ref qualifier"), cfoo.qualifier());
    }

    TEST("rvalue: lvalue_ref_qualifier") {
        Foo foo;
        ASSERT_EQ(std::string("lvalue ref qualifier"), foo.qualifier());
    }

    TEST("rvalue: rvalue_ref_qualifier") {
        ASSERT_EQ(std::string("rvalue ref qualifier"), Foo().qualifier());
    }
};

std::string&& operator+(std::string& lhs, const std::string& rhs)
{
    lhs += rhs;
    return std::move(lhs);
}

FIXTURE(LvalueSpec2) {    
    TEST("rvalue: functions_that_return_rvalue_ref") {
        auto &&str = std::string("c++") + "11";
        STATIC_ASSERT_RVALUE_REF(str);
    }
};

} // namespace

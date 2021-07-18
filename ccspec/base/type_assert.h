#include <type_traits>

#define STATIC_ASSERT_SAME_TYPE(expect_type, actual_type) \
    static_assert(std::is_same_v<expect_type, actual_type>)

#define STATIC_ASSERT_TYPE(expect_type, expr) \
    STATIC_ASSERT_SAME_TYPE(expect_type, decltype(expr))

#define STATIC_ASSERT_RVALUE_REF(expr) \
    static_assert(std::is_rvalue_reference_v<decltype(expr)>)

#define STATIC_ASSERT_LVALUE_REF(expr) \
    static_assert(std::is_lvalue_reference_v<decltype(expr)>)

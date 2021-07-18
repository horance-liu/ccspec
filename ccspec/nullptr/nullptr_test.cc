#include "cctest/cctest.h"
#include "ccspec/base/type_assert.h"

#include <memory>

namespace
{

FIXTURE(NullptrSpec) {
    TEST("the type of nullptr is std_nullptr_t") {
        STATIC_ASSERT_TYPE(std::nullptr_t, nullptr);
    }
};

FIXTURE(NullptrSpec2) {
    int expect_call = 0;

    void func(int value)
    {
        expect_call = 1;
    }

    void func(void* ptr)
    {
        expect_call = 2;
    }

    TEST("only nullptr is unambiguously a pointer") {
        func(0);
        ASSERT_EQ(1, expect_call);

        func(nullptr);
        ASSERT_EQ(2, expect_call);

        func((void*)NULL);
        ASSERT_EQ(2, expect_call);
    }
};

FIXTURE(NullptrSpec3) {
    std::unique_ptr<int> value_of(int value)
    {
        return std::make_unique<int>(value);
    }

    TEST("nullptr is more meanful than primitive 0 or NULL")
    {
        ASSERT_TRUE(value_of(100) != nullptr);

        ASSERT_TRUE(value_of(100) != 0);
        ASSERT_TRUE(value_of(100) != NULL);
    }
};

FIXTURE(NullptrSpec4) {
    template <typename Functor, typename Param>
    static void log(Functor f, Param p)
    {
        f(p);
    }

    static void fp(void*) {
        ASSERT_TRUE(true);
    }

    TEST("only nullptr can forward template")
    {
        log(fp, nullptr);

        // must be cast to void* 
        log(fp, (void*)0);
        log(fp, (void*)NULL);
    }
};

} // namespace
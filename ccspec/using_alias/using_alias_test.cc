#include "cctest/cctest.h"
#include "ccspec/base/type_assert.h"

#include <stack>
#include <list>
#include <array>
#include <map>
#include <unordered_map>

namespace {

FIXTURE(UsingAliasSpec1) {
    TEST("using alias can be equivalent to typedef") {
        typedef std::unordered_map<int, int> IntHashMap_1;
        using IntHashMap_2 = std::unordered_map<int, int>;

        STATIC_ASSERT_SAME_TYPE(IntHashMap_1, IntHashMap_2);
    }

    TEST("using alias can be more comprehensible than typedef") {
        typedef int (*Functor1)(int, int);
        using Functor2 = int(*)(int, int);

        STATIC_ASSERT_SAME_TYPE(Functor1, Functor2);
    }
};

FIXTURE(UsingAliasSpec2) {
    template <std::size_t N>
    using StringArray = std::array<std::string, N>;

    TEST("using alias can be used for partially bound template") {
        using expected_type = std::array<std::string, 3>;
        STATIC_ASSERT_SAME_TYPE(expected_type, StringArray<3>);
    }
};

FIXTURE(UsingAliasSpec3) {
    template <typename T>
    struct LinkedStack {
        typedef std::stack<T, std::list<T>> type;
    };

    template <typename T>
    struct Station {
        typename LinkedStack<T>::type stack;
    };

    TEST("no using alias, it must be preceded by typename for dependent type") {
        Station<int> station;
        STATIC_ASSERT_TYPE(Station<int>, station);
    }
};

FIXTURE(UsingAliasSpec4) {
    template <typename T>
    using LinkedStack = std::stack<T, std::list<T>>;

    template <typename T>
    struct Station {
        LinkedStack<T> stack;
    };

    TEST("using alias, no need for typename vanishes") {
        Station<int> station;
        STATIC_ASSERT_TYPE(Station<int>, station);
    }

};

FIXTURE(UsingAliasSpec5) {
    TEST("using alias is simpler than typedef") {
        STATIC_ASSERT_SAME_TYPE(int, std::remove_const<const int>::type);
        STATIC_ASSERT_SAME_TYPE(int, std::remove_const_t<const int>);
    }

    TEST("using alias is simpler than inner constexpr bool value") {
        static_assert(std::is_same<int, signed int>::value);
        static_assert(std::is_same_v<int, signed int>);
    }    
};

} // namespace
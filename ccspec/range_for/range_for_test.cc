#include "cctest/cctest.h"

#include <initializer_list>
#include <array>
#include <numeric>

namespace {

FIXTURE(RangeForSpec) {
    template <typename T, typename C>
    T sum(const C& c, T init = T())
    {
        for(const auto &e : c) {
            init += e;
        }

        return init;
    }

    TEST("range_for: all stl containers support range based for") {
        std::vector<int> v {1, 2, 3};

        ASSERT_EQ(6, sum<int>(v));
    }

    TEST("range_for: array supports range based for") {
        int arr[] = {1, 2, 3};

        ASSERT_EQ(6, sum<int>(arr));
    }

    TEST("range_for: list initializer supports range based for") {
        auto list = {1, 2, 3};
        
        ASSERT_EQ(6, sum<int>(list));
    }

    template <typename T, size_t MAX_NUM>
    struct FixedArray
    {
        FixedArray(std::initializer_list<T> list) {
            std::copy(list.begin(), list.end(), arr.begin());
        }

        T* begin()
        { return std::begin(arr); }

        T* end()
        { return std::end(arr); }

        const T* begin() const
        { return std::begin(arr); }

        const T* end() const
        { return std::end(arr); }

    private:
        std::array<T, MAX_NUM> arr;
    };    

    TEST("range_for: user defined type support range for only if begin and end are valid") {
        FixedArray<int, 3> array {1, 2, 3};

        ASSERT_EQ(6, sum<int>(array));
    }

    TEST("range_for: modify element when iterate list") {
        FixedArray<int, 3> array {1, 2, 3};

        for (auto &e : array)
        {
            e *= 2;
        }

        ASSERT_EQ(12, sum<int>(array));
    }

    TEST("range_for: use const reference when iterate class type") {
        std::vector<std::string> sv;

        sv.emplace_back("c++98");
        sv.emplace_back("|");
        sv.emplace_back("c++11");

        ASSERT_EQ("c++98|c++11", sum<std::string>(sv));
    }
};

} // namespace
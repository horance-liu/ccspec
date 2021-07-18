#include "cctest/cctest.h"
#include "ccspec//base/type_assert.h"

#include <initializer_list>
#include <array>

namespace {

FIXTURE(InitSpec) {
    TEST("direct initialization") {
        auto i(10);
        auto s("10");

        STATIC_ASSERT_TYPE(int, i);
        STATIC_ASSERT_TYPE(const char*, s);
    }

    TEST("copy initialization") {
        const auto i = 10;
        const auto s = std::string("10");

        STATIC_ASSERT_TYPE(const int, i);
        STATIC_ASSERT_TYPE(const std::string, s);
    }

    TEST("braced initialization for int") {
        int j = {10};
        int k {10};

        STATIC_ASSERT_TYPE(int, j);
        STATIC_ASSERT_TYPE(int, k);
    }

    TEST("braced initialization for std::string") {
        std::string t = {"10"};
        std::string v {"10"};

        STATIC_ASSERT_TYPE(std::string, t);
        STATIC_ASSERT_TYPE(std::string, v);
    }    
};

FIXTURE(AutoListInitSpec) {
    TEST("copy initialization with brace initialization is std::initializer_list") {
        auto i = {1};
        auto j = {1, 2, 3};

        STATIC_ASSERT_TYPE(std::initializer_list<int>, i);
        STATIC_ASSERT_TYPE(std::initializer_list<int>, j);
    }

    TEST("auto type for single brace initialization is not std::initializer_list in c++17") {
        auto i {1};
        STATIC_ASSERT_TYPE(int, i);
    }
};

FIXTURE(ArrayInitialization) {
    TEST("c style array supports the brace initialization: copy initializer") {
      int arr[] = {1, 2, 3};

      ASSERT_EQ(1, arr[0]);
      ASSERT_EQ(2, arr[1]);
      ASSERT_EQ(3, arr[2]);
    }

    TEST("c style array supports the brace initialization: direct initializer") {
      int arr[] {1, 2, 3};

      ASSERT_EQ(1, arr[0]);
      ASSERT_EQ(2, arr[1]);
      ASSERT_EQ(3, arr[2]);
    }

    TEST("the size brace initialization may be less than length of array") {
      int arr[3] = {1};

      ASSERT_EQ(1, arr[0]);
      ASSERT_EQ(0, arr[1]);
      ASSERT_EQ(0, arr[2]);
    }

    TEST("c99 initialization") {
      int arr[] = {
        [0] = 1,
        [2] = 3,
      };

      static_assert(3 == sizeof(arr) / sizeof(arr[0]));

      ASSERT_EQ(1, arr[0]);
      ASSERT_EQ(0, arr[1]);
      ASSERT_EQ(3, arr[2]);
    }
};

FIXTURE(ContainerInitialization) {
    TEST("std container supports the brace initialization") {
      std::vector<int> v1 = {1, 2, 3};
      std::vector<int> v2 {1, 2, 3};

      ASSERT_EQ(3, v1.size());
      ASSERT_EQ(3, v2.size());
    }

    template <typename T, typename U>
    void assert_each_with_value(const std::vector<T>& values, const U& expected) {
        for (auto &value : values) {
            ASSERT_EQ(expected, value);
        }
    }

    TEST("init vector using parenthes without default value") {
        std::vector<int> v(10);

        ASSERT_EQ(10, v.size());
        assert_each_with_value(v, 0);
    }

    TEST("init vector using parenthes with default value") {
        std::vector<int> v(10, 1);

        ASSERT_EQ(10, v.size());
        assert_each_with_value(v, 1);
    }

    TEST("init vector using brace with only one element") {
        std::vector<int> v {10};

        ASSERT_EQ(1, v.size());
        ASSERT_EQ(10, v[0]);
    }

    TEST("init vector using brace with only two elements") {
        std::vector<int> v {10, 1};

        ASSERT_EQ(2, v.size());
        ASSERT_EQ(10, v[0]);
        ASSERT_EQ(1,  v[1]);
    }

    TEST("init vector with string using brace with only one element") {
        std::vector<std::string> v {"hello"};

        ASSERT_EQ(1, v.size());
        ASSERT_EQ("hello", v[0]);
    }

    TEST("init vector with string using brace with only two elements") {
        std::vector<std::string> v {"hello", "world"};

        ASSERT_EQ(2, v.size());
        ASSERT_EQ("hello", v[0]);
        ASSERT_EQ("world", v[1]);
    }

    // If list initialization is not possible,
    // the compiler looks for other ways to
    // initialize the object from the given values.
    TEST("vector string initialization: using brace without default value") {
        std::vector<std::string> v {10};

        ASSERT_EQ(10, v.size());
        assert_each_with_value(v, "");
    }

    TEST("vector string initialization: using brace with default value") {
      std::vector<std::string> v{10, "hello"};

      ASSERT_EQ(10, v.size());
      assert_each_with_value(v, "hello");
    }
};

FIXTURE(StdArrayInitialization) {
    TEST("std array supports the brace initialization") {
      std::array<int, 3> arr1 {1, 2};

      ASSERT_EQ(3, arr1.size());
      ASSERT_EQ(0, arr1[arr1.size() - 1]);
    }
};

FIXTURE(AggregateInitSpec) {
    TEST("list_initialization: aggregate class supports the brace initialization") {
      struct Rectangle
      {
          int width;
          int height;
      } r = {1, 2};

      ASSERT_EQ(1,  r.width);
      ASSERT_EQ(2,  r.height);
    }
};

FIXTURE(NonAggregateInitialization) {
    TEST("list_initialization: non aggregate class supports the brace initialization")
    {
      struct Point
      {
          Point(int x, int y) : x(x), y(y)
          {}

          bool operator==(const Point& rhs) const
          {
              return x == rhs.x && y == rhs.y;
          }

      private:
          int x, y;
      } point {1, 2};

      ASSERT_EQ(Point(1, 2), point);
    }


    TEST("list_initialization: array in the class supports the brace initialization")
    {
      struct Widget
      {
          Widget() : data1 {1, 2}, p{new int[] {3, 4}}
          {}

          ~Widget() { delete [] p; }

          void assertValid()
          {
              ASSERT_EQ(1, data1[0]);
              ASSERT_EQ(2, data1[1]);

              ASSERT_EQ(1, data2[0]);
              ASSERT_EQ(0, data2[1]);

              ASSERT_EQ(0, data3[0]);
              ASSERT_EQ(0, data3[1]);

              ASSERT_EQ(3, p[0]);
              ASSERT_EQ(4, p[1]);
          }

      private:
          const int data1[2] {0};  // {0, 0}
          const int data2[2] {1};  // {1, 0}
          const int data3[2] {0};  // {0, 0}

          const int* p {nullptr};
      };

      Widget().assertValid();
    }

    TEST("list_initialization: return value support the brace initialization")
    {
      struct Line
      {
          Line(int start, int length)
              : start(start), length(length)
          {}

          static Line create(int start, int length)
          {
              return {start, length};
          }

          void assert_valid()
          {
              ASSERT_EQ(1,  start);
              ASSERT_EQ(10, length);
          }

      private:
          int start;
          int length;
      };

      Line::create(1, 10).assert_valid();
    }

    int sum(const std::initializer_list<int> numbers)
    {
        auto result = 0;
        for (auto num : numbers)
        {
            result += num;
        }
        return result;
    }

    TEST("list_initialization: function param support the brace initialization")
    {
      ASSERT_EQ(6, sum({1, 2, 3}));
    }

    TEST("list_initialization: empty braces mean no arguments but not an empty std::initializer_list")
    {
      struct Rectangle
      {
          Rectangle() = default;

          Rectangle(const std::initializer_list<int> values)
              : invoked(true)
          {}

          bool listInitialized() const
          { return invoked; }

      private:
          bool invoked = false;
      } rectangle1{}, rectangle2({}), rectangle3{{}};

      ASSERT_FALSE(rectangle1.listInitialized());
      ASSERT_TRUE(rectangle2.listInitialized());
      ASSERT_TRUE(rectangle2.listInitialized());
    }
    TEST("list_initialization: braced initializers are matched to std::initializer_list parameters if at all possible")

    {
      struct Widget
      {
          Widget(int, bool)
          {}

          Widget(const std::initializer_list<double>)
            : invoked(true)
          {}

          bool list_initialized() const
          { return invoked; }

      private:
          bool invoked { false };
      } widget1(1, true), widget2{1, true};

      ASSERT_FALSE(widget1.list_initialized());
      ASSERT_TRUE(widget2.list_initialized());
    }
};

} // namespace

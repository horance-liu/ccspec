#include "cctest/cctest.h"
#include <algorithm>

namespace {

FIXTURE(LambdaSpec1) {
    TEST("empty capture list") {
        std::vector<int> numbers = {-10, 0, 10};

        auto found = std::find_if(numbers.cbegin(), numbers.cend(), [](int value) { 
            return value > 0 && value < 20; 
        });

        ASSERT_NE(numbers.cend(), found);
    }
};

} // namespace


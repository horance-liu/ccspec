CMAKE_MINIMUM_REQUIRED(VERSION 3.18)

PROJECT(ccspec VERSION 1.0 LANGUAGES C CXX)

# Set c++ standard to c++17.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# enable ctest
enable_testing()

# target
file(GLOB_RECURSE srcs CONFIGURE_DEPENDS
    ccspec/*.cc
)

add_executable(ccspec ${srcs})

target_include_directories(ccspec
    PUBLIC .
)

target_link_libraries(ccspec
    PRIVATE cctest cctest_main
)

add_test(NAME ccspec COMMAND ccspec)

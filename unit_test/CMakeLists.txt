cmake_minimum_required(VERSION 3.18)

add_executable(ut
    main.cc
    malloc-allocator_test.cc
    array-allocator_test.cc
    heap-allocator_test.cc
    block-allocator_test.cc
)

target_include_directories(ut PRIVATE ../src)
#target_link_libraries(ut PRIVATE cpp-objectpool)
target_link_libraries(ut PRIVATE gtest)

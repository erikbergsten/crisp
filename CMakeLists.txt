cmake_minimum_required (VERSION 3.0)


# project (NAME HERE)

set(MUNIT_SRC "${PROJECT_SOURCE_DIR}/munit/munit.c")
set(MUNIT_INCLUDE "${PROJECT_SOURCE_DIR}/munit")

# uncomment or change if not using c99
set(CMAKE_C_FLAGS "-std=c99")


include_directories("${PROJECT_SOURCE_DIR}/public")

add_subdirectory(src)

add_subdirectory(tests)



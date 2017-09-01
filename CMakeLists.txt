cmake_minimum_required (VERSION 3.0)

project (crisp)

# uncomment or change if not using c99
set(CMAKE_C_FLAGS "-std=c99")

# uncomment if not debugging
add_definitions(-DCR_DEBUG)


include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/crisp_util/public")

add_subdirectory("crisp_util/src")
add_subdirectory(src)
add_subdirectory(tests)


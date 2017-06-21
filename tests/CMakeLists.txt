cmake_minimum_required (VERSION 3.0)

add_custom_target(tests)
if(TARGET tests)
  include_directories(${MUNIT_INCLUDE})
endif(TARGET tests)

macro(test name)
  add_executable("${name}_tests" "${name}_tests.c" ${MUNIT_SRC})
  add_dependencies(tests "${name}_tests")
  target_link_libraries("${name}_tests" crisp_util)
  target_link_libraries("${name}_tests" m)
endmacro(test)

# add all the tests here, test(file1) refers to the file file1_tests.c
test(file1)

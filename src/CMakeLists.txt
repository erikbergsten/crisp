cmake_minimum_required (VERSION 3.0)

# set name to PROJECT_NAME_SRC and list sources here
set(PROJECT_NAME_SRC "source1.c"
                     "source2.c" )

# project_name will be the name of teh .so file created
add_library(project_name SHARED ${PROJECT_NAME_SRC})

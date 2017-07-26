Crisp
=====

Build & Run
-----------

    mkdir build && cd build
    cmake -DCMAKE_BUILD_TYPE=Debug ..
    make
    ./tests/run_tests

Notes
-----

Lots of debug info is printed, to remove it unset the CR_DEBUG flag
in CMakeLists.txt and remake.

Building and running has only been tested on os x using clang.

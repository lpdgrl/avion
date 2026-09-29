# !/bin/bash

cd build 
cmake --build .
ctest --test-dir ./Tests --output-on-failure

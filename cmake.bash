#!/bin/bash

cmake -DCMAKE_INSTALL_PREFIX=~/University/cg/cw2/out/install/x64-clang-linux-debug -DCMAKE_CXX_COMPILER=/usr/bin/clang++ -DCMAKE_BUILD_TYPE=Debug -S ~/University/cg/cw2 -B ~/University/cg/cw2/out/build/x64-clang-linux-debug -G Ninja

cmake --build ~/University/cg/cw2/out/build/x64-clang-linux-debug --target main

~/University/cg/cw2/out/build/x64-clang-linux-debug/main

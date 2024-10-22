cmake_minimum_required(VERSION 3.20)
project(DS_AG)

set(CMAKE_CXX_STANDARD 20)

add_executable(DS_AG
        sort.cpp
        include/stdc++.h
)

add_executable(mgraph
    source/mgraph.cpp
    mgraph_main.cpp
    include/mgraph.h
    include/stdc++.h
)

add_executable(algraph
    source/algraph.cpp
    algraph_main.cpp
    include/stdc++.h
    include/algraph.h
)

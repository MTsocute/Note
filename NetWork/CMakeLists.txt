cmake_minimum_required(VERSION 3.5.0)
project(cpp_demo VERSION 0.1.0 LANGUAGES C CXX)


# 设定为 C++20 版本
set(CMAKE_CXX_STANDARD 20)

add_executable(cpp_demo main.cpp)

add_executable(mutal_V
        data_cpt.cpp)

add_executable(lock_template_class
        lock.cpp)

add_executable(call_one_demo
        单例模式.cpp
        include/stdc++.h
)

add_executable(condition_Var
        条件变量.cpp
        include/stdc++.h
)

add_executable(thread_pool
        线程池.cpp
        include/stdc++.h
)


add_executable(CPP_NEW
        CPP_new_feature.cpp)


add_executable(thread_pool2
        threadPool.cpp
        include/stdc++.h
)


add_executable(asysnc
        IO_async.cpp
        include/stdc++.h
)


add_executable(atomicop
        ato.cpp
        include/stdc++.h
)
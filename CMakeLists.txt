cmake_minimum_required(VERSION 3.25)

project("CppDataAlgo")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_executable(CppDataAlgo src/main.cpp
        inc/array.hpp
        inc/box_ptr.hpp
        inc/cont.hpp
        inc/forward_list.hpp
        inc/iterator.hpp
        inc/span.hpp
        inc/vector.hpp
        inc/rc_ptr.hpp
)

include(CPM.cmake)
CPMAddPackage("gh:fmtlib/fmt#10.0.0")

target_link_libraries(CppDataAlgo PRIVATE fmt)
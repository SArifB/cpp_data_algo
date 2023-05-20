cmake_minimum_required(VERSION 3.25)

project("CppDataAlgo")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_executable(CppDataAlgo src/main.cpp
        containers/array.hpp
        pointers/box_ptr.hpp
        containers/inc.hpp
        containers/forward_list.hpp
        utilities/iterator.hpp
        containers/view.hpp
        containers/vector.hpp
        pointers/rc_ptr.hpp
        pointers/ref_ptr.hpp
        utilities/pair.hpp
        utilities/utility.hpp
        containers/static_vector.hpp
        containers/suoc_map.hpp
        utilities/option.hpp
        containers/range.hpp)

include(CPM.cmake)
CPMAddPackage("gh:fmtlib/fmt#10.0.0")

target_link_libraries(CppDataAlgo PRIVATE fmt)
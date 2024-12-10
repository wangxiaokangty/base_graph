cmake_minimum_required(VERSION 3.28)
project(base_graph)

set(CMAKE_CXX_STANDARD 17)


add_executable(base_graph main.cpp
        src/BaseGraph.cpp
        include/BaseGraph.h
        src/RandomRange.cpp
        include/RandomRange.h
        src/TlsGraph.cpp
        include/TlsGraph.h)
add_executable(test test.cpp src/BaseGraph.cpp
        include/BaseGraph.h
        src/RandomRange.cpp
        include/RandomRange.h
        src/TlsGraph.cpp
        include/TlsGraph.h)


set_target_properties(base_graph PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)
set_target_properties(test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

cmake_minimum_required(VERSION 3.16)
project(reachability)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
# set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

include_directories(.)
include_directories(include)
include_directories(algorithms/OreachUtils)
include_directories(algorithms/OreachUtils/extern/argtable3-3.0.3)
include_directories(algorithms/OreachUtils/extern/KaHIP/lib)
include_directories(algorithms/OreachUtils/extern/KaHIP/lib/tools)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    message(STATUS "OpenMP support detected")
    add_definitions(${OpenMP_CXX_FLAGS})
else()
    message(WARNING "OpenMP not available, activating workaround")
    add_library(OpenMP::OpenMP_CXX IMPORTED INTERFACE)
    set_property(TARGET OpenMP::OpenMP_CXX PROPERTY INTERFACE_COMPILE_OPTIONS "")
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/misc)
endif()

file(GLOB_RECURSE OREACH_SOURCES
        algorithms/OreachUtils/*.h
        algorithms/OreachUtils/*.cpp
        algorithms/OreachUtils/*.c
        algorithms/OreachUtils/*.hpp
)

add_executable(
        reachability
        main.cpp
        Graph.cpp
        Timer.cpp
        AutoTest.cpp
        algorithms/GrailUtils/Grail.cpp
        algorithms/GrailUtils/GrailGraph.cpp
        algorithms/GrailUtils/GrailGraphUtil.cpp
        algorithms/GrailUtils/interval_tree.cpp
        algorithms/GrailUtils/TCSEstimator.cpp
        algorithms/PathTreeUtils/DWGraph.cpp
        algorithms/PathTreeUtils/DWGraphUtil.cpp
        algorithms/PathTreeUtils/PathTree.cpp
        algorithms/PathTreeUtils/PathTreeGraph.cpp
        algorithms/PathTreeUtils/PathTreeGraphUtil.cpp
        algorithms/BFL.cpp
        algorithms/GrailWrapper.cpp
        algorithms/RE_Toy.cpp
        algorithms/PathTreeWrapper.cpp
        algorithms/TOLWrapper.cpp
        algorithms/GrippWrapper.cpp
        algorithms/GrippUtils/GrippGraph.cpp
        algorithms/GrippUtils/Gripp.cpp
        algorithms/Ferrari.cpp
        algorithms/FerrariUtils/FerrariGraph.cpp
        algorithms/FerrariUtils/Index.cpp
        algorithms/FerrariUtils/IntervalList.cpp
        algorithms/IP.cpp
        algorithms/IpUtils/IPGraph.cpp
        algorithms/IpUtils/Util.cpp
        algorithms/PLL.cpp
        algorithms/PllUtils/PrunedLabeling.cpp
        algorithms/PReaCH.cpp
        algorithms/PreachUtils/ConstructionGraph.cc
        algorithms/PreachUtils/Fifo.cc
        algorithms/PreachUtils/QueryGraph.cc
        algorithms/OReachWrapper.cpp
        ${OREACH_SOURCES}
)

target_compile_options(reachability PRIVATE -O0)

cmake_minimum_required(VERSION 3.16)
project(Kang)

set(CMAKE_CXX_STANDARD 14)

# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
# SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
# SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

include_directories(.)
include_directories(algorithms)
include_directories(algorithms/GrailUtils)
include_directories(algorithms/PathTreeUtils)

add_executable(xJump
        algorithms/GrailUtils/exception_list.h
        algorithms/GrailUtils/exception_list_incremental_base.h
        algorithms/GrailUtils/exception_list_incremental_plus.h
        algorithms/GrailUtils/Grail.cpp
        algorithms/GrailUtils/Grail.h
        algorithms/GrailUtils/GrailGraph.cpp
        algorithms/GrailUtils/GrailGraph.h
        algorithms/GrailUtils/GrailGraphUtil.cpp
        algorithms/GrailUtils/GrailGraphUtil.h
        algorithms/GrailUtils/interval_tree.cpp
        algorithms/GrailUtils/interval_tree.h
        algorithms/GrailUtils/TCSEstimator.cpp
        algorithms/GrailUtils/TCSEstimator.h
        algorithms/GrailUtils/templatestack.h
        algorithms/GrailUtils/time_tracker.h
        algorithms/GrailUtils/utils.h
        algorithms/PathTreeUtils/DWGraph.cpp
        algorithms/PathTreeUtils/DWGraph.h
        algorithms/PathTreeUtils/DWGraphUtil.cpp
        algorithms/PathTreeUtils/DWGraphUtil.h
        algorithms/PathTreeUtils/PathTree.cpp
        algorithms/PathTreeUtils/PathTree.h
        algorithms/PathTreeUtils/PathTreeGraph.cpp
        algorithms/PathTreeUtils/PathTreeGraph.h
        algorithms/PathTreeUtils/PathTreeGraphUtil.cpp
        algorithms/PathTreeUtils/PathTreeGraphUtil.h
        algorithms/TOLWrapper.cpp
        algorithms/TOLWrapper.h
        algorithms/TolUtils/utility1.h
        algorithms/TolUtils/Butterfly.h
        algorithms/GrippWrapper.cpp
        algorithms/GrippWrapper.h
        algorithms/GrippUtils/Util.h
        algorithms/GrippUtils/GrippGraph.cpp
        algorithms/GrippUtils/GrippGraph.h
        algorithms/GrippUtils/Gripp.cpp
        algorithms/GrippUtils/Gripp.h
        algorithms/Algorithm.h
        algorithms/BFL.cpp
        algorithms/BFL.h
        algorithms/GrailWrapper.cpp
        algorithms/GrailWrapper.h
        algorithms/Kang.cpp
        algorithms/Kang.h
        algorithms/PathTreeWrapper.cpp
        algorithms/PathTreeWrapper.h
        AutoTest.cpp
        AutoTest.h
        Graph.cpp
        Graph.h
        main.cpp
        Profile.h )

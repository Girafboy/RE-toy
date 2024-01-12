cmake_minimum_required(VERSION 3.16)
project(reachability)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)
include_directories(include)


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
        algorithms/DblUtils/DBL.cc
        algorithms/DBLWrapper.cpp)

target_compile_options(reachability PRIVATE -O0)

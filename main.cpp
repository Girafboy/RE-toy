#include <iomanip>
#include <iostream>
#include <ostream>
#include <queue>
#include <chrono>
#include <fstream>
#include <filesystem>

#include "Graph.h"
#include "AutoTest.h"
#include "Profile.h"

#include "algorithms/Kang.h"
#include "algorithms/BFL.h"
#include "algorithms/GrailWrapper.h"
#include "algorithms/PathTreeWrapper.h"
#include "algorithms/TOLWrapper.h"
#include "algorithms/GrippWrapper.h"
#include "algorithms/buTRWrapper.h"
#include "algorithms/Ferrari.h"
#include "algorithms/IP.h"
#include "algorithms/PLL.h"
#include "algorithms/TFLabel.h"
#include "algorithms/TFL.h"
#include "algorithms/PReaCH.h"
#include "algorithms/DBLWrapper.h"

using kang::Kang;
using bfl::BFL;
using grail::GrailWrapper;
using path_tree::PathTreeWrapper;
using tol::TOLWrapper;
using gripp::GrippWrapper;
//using butr::buTRWrapper;
using ferrari::Ferrari;
using ip::IP;
using pll::PLL;
//using tf_label::TFLabel;
//using tfl::TFL;
using preach::PReaCH;
using dbl::DBLWrapper;


Profile testAlgorithmsOnGraph(const Graph &graph, Algorithm *algorithm, int check_reachable_times, bool check_only_reached=false) {
    Profile profile;
    profile.graph_name = graph.getName();
    profile.algorithm_name = algorithm->getName();
    profile.params = algorithm->getParams();

    decltype(std::chrono::high_resolution_clock::now()) time1, time2;
    std::chrono::duration<double, std::milli> ms_double{};

    time1 = std::chrono::high_resolution_clock::now();
    algorithm->construction(graph);
    time2 = std::chrono::high_resolution_clock::now();
    ms_double = time2 - time1;
    profile.preparation_time = ms_double.count();

    AutoTest auto_test = AutoTest(&graph, algorithm);
    auto_test.generateQueries(check_reachable_times, check_only_reached);
    time1 = std::chrono::high_resolution_clock::now();
    auto_test.runQueryTest();
    time2 = std::chrono::high_resolution_clock::now();
    ms_double = time2 - time1;
    profile.total_has_path_time = ms_double.count();

    profile.index_size = algorithm->getIndexSize();
    algorithm->reset();

    return profile;
}

void main_test() {
    std::vector<Algorithm *> algorithms = {
        /****************************
         * Kang
         ****************************/ 
        new Kang(32),
        new Kang(64),
        new Kang(128),
        new Kang(256),
//        new Kang(512),
//        new Kang(1024),
//        new Kang(2048),
//        new Kang(4096),
        // new Kang(1024*16),
        /****************************
         * BFL
         ****************************/ 
//        new BFL(1),
//        new BFL(2),
//        new BFL(5),
//        new BFL(10),
//        new BFL(20),
//        new BFL(50),
//        new BFL(100),
//        new BFL(200),
//        new BFL(500),
        /****************************
         * Grail
         ****************************/ 
//        new GrailWrapper(1, 0, 2),
//        new GrailWrapper(1, 0, 3),
//        new GrailWrapper(1, 0, 4),
//        new GrailWrapper(1, 0, 5),
//        new GrailWrapper(1, 1, 2),
//        new GrailWrapper(1, 1, 3),
//        new GrailWrapper(1, 1, 4),
//        new GrailWrapper(1, 1, 5),
//        new GrailWrapper(-2, 0, 2),
//        new GrailWrapper(-2, 0, 3),
//        new GrailWrapper(-2, 0, 4),
//        new GrailWrapper(-2, 0, 5),
//        new GrailWrapper(-2, 1, 2),
//        new GrailWrapper(-2, 1, 3),
//        new GrailWrapper(-2, 1, 4),
//        new GrailWrapper(-2, 1, 5),
        /****************************
         * Path Tree
         ****************************/ 
//        new PathTreeWrapper(1),
//        new PathTreeWrapper(2),
//        new PathTreeWrapper(3),
//        new PathTreeWrapper(4),
        /****************************
         * TOL
         ****************************/ 
//        new TOLWrapper(0, 1),
//        new TOLWrapper(0, 2),
//        new TOLWrapper(1, 1),  // todo: 正确性有问题
//        new TOLWrapper(1, 2),
        /****************************
         * Gripp
         ****************************/ 
//        new GrippWrapper(),  // 极慢
        /****************************
         * buTR
            TODO: buTR的实现有问题
         ****************************/ 
        // new buTRWrapper(),
        /****************************
         * Ferrari
         ****************************/ 
//        new Ferrari(2, 32, true),
        /****************************
         * IP
            TODO: BUG
         ****************************/ 
//         new IP(2, 2),
        /****************************
         * PLL
         ****************************/ 
//        new PLL(true),
//        new PLL(false),
        /****************************
         * TF-label
            TODO: 有bug，随机性强，偶尔跑通也慢，不知道是不是实现问题
            Algorithm TOL on Graph random-100-10 has a preparation time of 0.995833 ms, an total 'has_path' time of 3.10062 ms, and an index space of 10060 B.
            Algorithm TF-label on Graph random-100-10 has a preparation time of 4.97579 ms, an total 'has_path' time of 52.8992 ms, and an index space of 18360 B.
         ****************************/ 
        // new TFLabel(),
        // new TFL(),
        /****************************
         * PReaCH
         ****************************/ 
//        new PReaCH(),
        /****************************
         * DBL
         ****************************/ 
//        new DBLWrapper(),
    };

    /***************************************** Correctness Test *****************************************/
    std::vector<Graph> graphs_for_correctness_test = {
            Graph(5, 1, "random-5-1"),
            Graph(100, 10, "random-100-10"),
    };
    for (auto algorithm: algorithms) {
        for (const auto &graph : graphs_for_correctness_test) {
            algorithm->construction(graph);
            AutoTest autoTest(&graph, algorithm);
            assert(autoTest.checkCorrectness().first);
            algorithm->reset();
        }
        std::cout << "Correctness Test Pass! (" << algorithm->getName() << ", " << algorithm->getParams() << ")" << std::endl;
    }
    /***************************************** Correctness Test *****************************************/

    /***************************************** Perfomance Test *****************************************/
    Profile profile;

    int check_reachable_times = 100000;

    std::vector<Graph> graphs = {
        // Graph(100, 10, "random-100-10"),
        Graph(1000, 1, "random-1000-1"),
//        Graph(1000, 2, "random-1000-2"),
//        Graph(1000, 5, "random-1000-5"),
//        Graph(1000, 10, "random-1000-10"),
//        Graph(1000, 20, "random-1000-20"),
//        Graph(1000, 50, "random-1000-50"),
        Graph(1000, 100, "random-1000-100"),
        // Graph(10000, 10, "random-10000-10"),
        // Graph(100000, 10, "random-100000-10"),
        // Graph("../graphs/large_real/citeseer_scc.txt", "citeseer_scc"),
        // Graph("../graphs/small_real_dense/arXiv_sub_6000-1.txt", "arXiv_sub_6000-1")
    };


    std::ofstream myfile;
    myfile.open("../output/result.csv");
    myfile << "algorithm,graph,params,construction(ms),index(B),query(us)\n";
    std::cout << std::setw(20) << "Algorithm" << std::setw(20) << "Graph" << std::setw(50) << "Params" << std::setw(20) << "Construction(ms)" << std::setw(20) << "Index(B)" << std::setw(20) << "Query(us)" << std::endl;
    for (const auto &graph: graphs) {
        for (auto algorithm: algorithms) {
            profile = testAlgorithmsOnGraph(graph, algorithm, check_reachable_times);

            myfile << profile.algorithm_name << ","
                << profile.graph_name << ","
                << profile.params << ","
                << profile.preparation_time << ","
                << profile.index_size << ","
                << profile.total_has_path_time * 1000 / check_reachable_times << "\n";
            std::cout << std::setw(20) << profile.algorithm_name 
                << std::setw(20) << profile.graph_name 
                << std::setw(50) << profile.params 
                << std::setw(20) << profile.preparation_time 
                << std::setw(20) << profile.index_size 
                << std::setw(20) << profile.total_has_path_time * 1000 / check_reachable_times << std::endl;
        }
    }
    myfile.close();
    /***************************************** Perfomance Test *****************************************/

    for (auto algorithm: algorithms) {
        delete algorithm;
    }
}

int main() {
    main_test();

    return 0;
}

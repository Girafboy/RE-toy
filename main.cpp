#include <iostream>
#include <queue>
#include <chrono>
#include <fstream>

#include "algorithms/Kang.h"
#include "Graph.h"
#include "AutoTest.h"
#include "Profile.h"
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
using butr::buTRWrapper;
using ferrari::Ferrari;
using ip::IP;
using pll::PLL;
using tf_label::TFLabel;
using tfl::TFL;
using preach::PReaCH;
using dbl::DBLWrapper;


Profile
testAlgorithmsOnGraph(const Graph &graph, Algorithm *algorithm, int check_reachable_times, bool check_correctness,
                      bool check_only_reached) {
    Profile profile;
    profile.graph_name = graph.getName();
    profile.algorithm_name = algorithm->getName();
    profile.params = algorithm->getParams();

//    algorithm->reset();
    decltype(std::chrono::high_resolution_clock::now()) time1, time2;
    std::chrono::duration<double, std::milli> ms_double{};

    time1 = std::chrono::high_resolution_clock::now();
    algorithm->construction(graph);
    time2 = std::chrono::high_resolution_clock::now();
    ms_double = time2 - time1;
    profile.preparation_time = ms_double.count();

    AutoTest auto_test = AutoTest(&graph, algorithm);
    profile.total_has_path_time = auto_test.runAutoTest(check_reachable_times, check_correctness, check_only_reached);
    profile.index_size = algorithm->getIndexSize();
    algorithm->reset();

    return profile;
}

void main_test() {
    std::vector<Algorithm *> algorithms = {
            // Kang
            new Kang(32),
//            new XJump(64),
//            new XJump(128),
//            new XJump(256),
//            new XJump(512),
//            new XJump(1024),
//            new XJump(2048),
//            new XJump(4096),
            // BFL
            new BFL(1),
//            new BFL(2),
//            new BFL(5),
//            new BFL(10),
//            new BFL(20),
//            new BFL(50),
//            new BFL(100),
//            new BFL(200),
//            new BFL(500),
            // Grail
            new GrailWrapper(1, 0, 2),
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
            // Path Tree
            new PathTreeWrapper(1),
//        new PathTreeWrapper(2),
//        new PathTreeWrapper(3),
//        new PathTreeWrapper(4),
            // TOL
            new TOLWrapper(0, 1),
//        new TOLWrapper(0, 2),
//        new TOLWrapper(1, 1),  // todo: 正确性有问题
//        new TOLWrapper(1, 2),
            // Gripp
//            new GrippWrapper(),  // 极慢
            // buTR
            // todo: buTR的实现有问题
//        new buTRWrapper(),
            // Ferrari
        new Ferrari(2, 32, true),
            // IP
            // BUG
        // new IP(2, 2),
            // PLL
        new PLL(true),
        new PLL(false),
            // TF-label
            // todo: 有bug，随机性强，偶尔跑通也慢，不知道是不是实现问题
            // Algorithm TOL on Graph random-100-10 has a preparation time of 0.995833 ms, an total 'has_path' time of 3.10062 ms, and an index space of 10060 B.
            // Algorithm TF-label on Graph random-100-10 has a preparation time of 4.97579 ms, an total 'has_path' time of 52.8992 ms, and an index space of 18360 B.
//        new TFLabel(),
//        new TFL(),
            // PReaCH
        new PReaCH(),
            // DBL
        new DBLWrapper(),
    };

    /***************************************** Correctness Test *****************************************/
    for (auto algorithm: algorithms) {
        testAlgorithmsOnGraph(Graph("../graphs/graph.txt", "simple-graph"), algorithm, 100, true, false);
//        testAlgorithmsOnGraph(Graph(3, 1, "random-3-1"), algorithm, 100, true, false);
//        testAlgorithmsOnGraph(Graph(10, 2, "random-10-2"), algorithm, 100, true, false);
//        testAlgorithmsOnGraph(Graph(20, 3, "random-20-3"), algorithm, 100, true, false);
//        testAlgorithmsOnGraph(Graph(63, 8, "random-63-8"), algorithm, 100, true, false);
//        testAlgorithmsOnGraph(Graph(64, 8, "random-64-8"), algorithm, 100, true, false);
//        testAlgorithmsOnGraph(Graph(65, 8, "random-65-8"), algorithm, 100, true, false);
        testAlgorithmsOnGraph(Graph(100, 10, "random-100-10"), algorithm, 100, true, false);
//        testAlgorithmsOnGraph(Graph(1000, 10, "random-1000-10"), algorithm, 100, true, false);
        std::cout << "Correctness Test Pass! (" << algorithm->getName() << ", " << algorithm->getParams() << ")"
                  << std::endl;
    }
    /***************************************** Correctness Test *****************************************/

    /***************************************** Perfomance Test *****************************************/
    std::vector<Profile> profiles;

    int check_reachable_times = 1000;
    bool check_only_reached = false;

    // random graphs
    std::vector<Graph> graphs = {
//            Graph(100, 10, "random-100-10"),
//            Graph(1000, 10, "random-1000-10"),
//            Graph(10000, 10, "random-10000-10"),
//            Graph(100000, 10, "random-100000-10"),
    };
    for (const auto &graph: graphs) {
        for (auto algorithm: algorithms) {
            std::cout << "Testing... (" << graph.getName() << ", " << algorithm->getName() << ", "
                      << algorithm->getParams() << ")" << std::endl;
            profiles.push_back(
                    testAlgorithmsOnGraph(graph, algorithm, check_reachable_times, false, check_only_reached));
        }
    }

    // real graphs
//    const std::filesystem::path real_dataset{"../datasets"};
//    const std::string ext = ".gra";
//    std::ifstream in;
//    for (const auto &dir_entry : std::filesystem::recursive_directory_iterator{real_dataset}) {
//        if (dir_entry.path().extension() == ext) {
//            in.open(dir_entry.path());
//            Graph graph(in, dir_entry.path().filename().stem().string());
//            in.close();
//            for (auto algorithm: algorithms) {
//                std::cout << "Testing... (" << graph.getName() << ", " << algorithm->getName() << ", "
//                          << algorithm->getParams() << ")" << std::endl;
//                profiles.push_back(
//                        testAlgorithmsOnGraph(graph, algorithm, check_reachable_times, false, check_only_reached));
//            }
//        }
//    }

    for (const auto &profile: profiles) {
        std::cout << "Algorithm " << profile.algorithm_name << " on Graph " << profile.graph_name
                  << " has a preparation time of " << profile.preparation_time << " ms, "
                  << "an total 'has_path' time of " << profile.total_has_path_time << " ms, "
                  << "and an index space of " << profile.index_size << " B."
                  << std::endl;
    }
    std::ofstream myfile;
    myfile.open("../output/example.csv");
    myfile << "algorithm,graph,params,construction(ms),index(B),query(ms/" << check_reachable_times << ")\n";
    for (const auto &profile: profiles) {
        myfile << profile.algorithm_name << ","
               << profile.graph_name << ","
               << profile.params << ","
               << profile.preparation_time << ","
               << profile.index_size << ","
               << profile.total_has_path_time << "\n";
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

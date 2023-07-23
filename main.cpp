#include <iostream>
#include <queue>
#include <chrono>
#include <fstream>
#include <climits>

#include "algorithms/Kang.h"
#include "Graph.h"
#include "AutoTest.h"
#include "Profile.h"
#include "algorithms/BFL.h"
#include "algorithms/GrailWrapper.h"
#include "algorithms/PathTreeWrapper.h"
#include "algorithms/TOLWrapper.h"
#include "algorithms/GrippWrapper.h"

using kang::Kang;
using bfl::BFL;
using grail::GrailWrapper;
using path_tree::PathTreeWrapper;
using tol::TOLWrapper;
using gripp::GrippWrapper;


Profile testAlgorithmsOnGraph(const Graph &graph, Algorithm* algorithm, int check_reachable_times, bool check_correctness, bool check_only_reached) {
    Profile profile;
    profile.graph_name = graph.getName();
    profile.algorithm_name = algorithm->getName();
    profile.params = algorithm->getParams();

    algorithm->reset();
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

    return profile;
}


int main() {
    std::vector<Algorithm*> algorithms = {
        // Kang
        new Kang(32),
        new Kang(64),
        new Kang(128),
        new Kang(256),
        new Kang(512),
        new Kang(1024),
        new Kang(2048),
        new Kang(4096),
        // BFL
        new BFL(),
        // Grail
        new GrailWrapper(1, 0, 2),
        new GrailWrapper(1, 0, 3),
        new GrailWrapper(1, 0, 5),
        new GrailWrapper(1, 0, 10),
        new GrailWrapper(1, 0, 20),
        new GrailWrapper(1, 0, 50),
        new GrailWrapper(1, 0, 100),
        new GrailWrapper(1, 0, 200),
        new GrailWrapper(1, 0, 500),
        // Path Tree
        new PathTreeWrapper(1),
        // TOL
        new TOLWrapper(0, 2),
        new TOLWrapper(1, 2),
        // Gripp
        new GrippWrapper(),
    };

    /***************************************** Correctness Test *****************************************/
    for (auto algorithm : algorithms) {
        testAlgorithmsOnGraph(Graph("../graphs/graph.txt", "simple-graph"), algorithm, 100, true, false);
        testAlgorithmsOnGraph(Graph(100, 10, "random-100-10"), algorithm, 100, true, false);
        std::cout << "Correctness Test Pass! (" << algorithm->getName() << "\t" << algorithm->getParams() << ")" << std::endl;
    }
    /***************************************** Correctness Test *****************************************/

    /***************************************** Perfomance Test *****************************************/
    std::vector<Profile> profiles;

    std::vector<Graph> graphs = {
        // Graph(100, 10, "random-100-10"),
        // Graph(300, 10, "random-300-10"),
        Graph(1000, 1, "random-1000-1"),
        Graph(1000, 2, "random-1000-2"),
        Graph(1000, 5, "random-1000-5"),
        Graph(1000, 10, "random-1000-10"),
        Graph(1000, 20, "random-1000-20"),
        Graph(1000, 50, "random-1000-50"),
        Graph(1000, 100, "random-1000-100"),
        // Graph(10000, 100, "random-10000-10"),
        // Graph(100000, 10, "random-100000-10")
    };
    int check_reachable_times = 100000;
    bool check_only_reached = false;
    for (const auto& graph : graphs) {
        for (auto algorithm : algorithms) {
            std::cout << "Testing... (" << graph.getName() << "\t" << algorithm->getName() << "\t" << algorithm->getParams() << ")" << std::endl;
            profiles.push_back(testAlgorithmsOnGraph(graph, algorithm, check_reachable_times, false, check_only_reached));
        }
    }

    for (const auto &profile : profiles) {
        std::cout << "Algorithm " << profile.algorithm_name << " on Graph " << profile.graph_name
                  << " has a preparation time of " << profile.preparation_time << " ms, "
                  << "an total 'has_path' time of " << profile.total_has_path_time << " ms, "
                  << "and an index space of " << profile.index_size << " B."
                  << std::endl;
    }
    std::ofstream myfile;
    myfile.open ("../output/example.csv");
    myfile << "algorithm,graph,params,construction(ms),index(B),query(ms/" << check_reachable_times << ")\n";
    for (const auto &profile : profiles) {
        myfile << profile.algorithm_name << ","
               << profile.graph_name << ","
               << profile.params << ","
               << profile.preparation_time << ","
               << profile.index_size << ","
               << profile.total_has_path_time << "\n";
    }
    myfile.close();
    /***************************************** Perfomance Test *****************************************/

    for (auto algorithm : algorithms) {
        delete algorithm;
    }

    return 0;
}

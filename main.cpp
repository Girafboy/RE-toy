#include <iomanip>
#include <iostream>
#include <ostream>
#include <queue>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <cstring>
#include <numeric>
#include <sstream>

#include "Graph.h"
#include "AutoTest.h"
#include "Profile.h"

#include "algorithms/ORSE_Toy.h"
#include "algorithms/BFL.h"
#include "algorithms/GrailWrapper.h"
#include "algorithms/PathTreeWrapper.h"
#include "algorithms/TOLWrapper.h"
#include "algorithms/GrippWrapper.h"
//#include "algorithms/buTRWrapper.h"
#include "algorithms/Ferrari.h"
#include "algorithms/IP.h"
#include "algorithms/PLL.h"
//#include "algorithms/TFLabel.h"
//#include "algorithms/TFL.h"
#include "algorithms/PReaCH.h"
#include "algorithms/DBLWrapper.h"

using orse_toy::ORSE_Toy;
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


decltype(std::chrono::high_resolution_clock::now()) start_time;
unsigned int max_time_second = 0;


//Profile testAlgorithmsOnGraph(const Graph &graph, Algorithm *algorithm, int check_reachable_times, bool check_only_reached=false) {
//    Profile profile;
//    profile.graph_name = graph.getName();
//    profile.algorithm_name = algorithm->getName();
//    profile.params = algorithm->getParams();
//
//    decltype(std::chrono::high_resolution_clock::now()) time1, time2;
//    std::chrono::duration<double, std::milli> ms_double{};
//
//    time1 = std::chrono::high_resolution_clock::now();
//    algorithm->construction(graph);
//    time2 = std::chrono::high_resolution_clock::now();
//    ms_double = time2 - time1;
//    profile.preparation_time = ms_double.count();
//
//    AutoTest auto_test = AutoTest(&graph, algorithm);
//    auto_test.generateQueries(check_reachable_times, check_only_reached);
//    time1 = std::chrono::high_resolution_clock::now();
//    auto_test.runQueryTest();
//    time2 = std::chrono::high_resolution_clock::now();
//    ms_double = time2 - time1;
//    profile.total_has_path_time = ms_double.count();
//
//    profile.index_size = algorithm->getIndexSize();
//    algorithm->reset();
//
//    return profile;
//}

void testAccuracy() {
    ORSE_Toy algorithm(32, 2.0);
    int n = 1000, d = 10;
    Graph graph(n, d, "random");
    algorithm.construction(graph);
    AutoTest autoTest(&graph, &algorithm);
    auto ret = autoTest.checkCorrectness();
    if (ret.first) {
        std::cout << "Correctness test passed." << std::endl;
    } else {
        std::cout << "Correctness test failed." << std::endl;
    }
}

Profile testAlgorithmsOnGraph(const Graph &graph, Algorithm *algorithm, int check_reachable_times, bool check_only_reached=false) {
    Profile profile;
    profile.graph_name = graph.getName();
    profile.algorithm_name = algorithm->getName();
    profile.params = algorithm->getParams();

    decltype(std::chrono::high_resolution_clock::now()) time1, time2;
    std::chrono::duration<long long, std::nano> ms_nano{};

    time1 = std::chrono::high_resolution_clock::now();
    algorithm->construction(graph);
    time2 = std::chrono::high_resolution_clock::now();
    ms_nano = time2 - time1;
    profile.preparation_time_ns = ms_nano.count();

    profile.index_size = algorithm->getIndexSize();

    std::vector<long long> has_path_times_ns;
    AutoTest auto_test = AutoTest(&graph, algorithm);
    auto queries = auto_test.generateQueries(check_reachable_times, check_only_reached);
    for (const auto &query : queries) {
        std::vector<long long> tmp_has_path_times_ns;
        for (int _ = 0; _ < 5; ++_) {
            time1 = std::chrono::high_resolution_clock::now();
            algorithm->TC_haspath(query.first, query.second);
            time2 = std::chrono::high_resolution_clock::now();
            ms_nano = time2 - time1;
            tmp_has_path_times_ns.push_back(ms_nano.count());
        }
        std::nth_element(tmp_has_path_times_ns.begin(), tmp_has_path_times_ns.begin() + tmp_has_path_times_ns.size() / 2, tmp_has_path_times_ns.end());
        has_path_times_ns.push_back(tmp_has_path_times_ns[tmp_has_path_times_ns.size() / 2]);
        ms_nano = time2 - start_time;
        if ((unsigned long long)max_time_second * 1000000000 - ms_nano.count() < 2000000000) {
            break;
        }
    }
    profile.query_num = has_path_times_ns.size();
    if (has_path_times_ns.empty()) {
        return profile;
    }

    long long sum = std::accumulate(has_path_times_ns.begin(), has_path_times_ns.end(), 0ll);
    double mean = (double)sum / (double)has_path_times_ns.size();
    profile.average_has_path_time_ns = mean;

//    std::vector<double> diff(has_path_times_ns.size());
//    std::transform(has_path_times_ns.begin(), has_path_times_ns.end(), diff.begin(), [mean](double x) { return x - mean; });
//    double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
//    double stdev = std::sqrt(sq_sum / (double)has_path_times_ns.size());
//    profile.standard_deviation_of_has_path_times_ns = stdev;

    profile.has_path_times_ns = std::move(has_path_times_ns);

    return profile;
}


void usage() {
    std::string h = "reachability - A program for running reachability algorithms on a given directed graph\n"
                    "\n"
                    "Usage:\n"
                    "./reachability [options] --time <max_execution_time>\n"
                    "\n"
                    "Options:\n"
                    "--help                     Display this help message and exit\n"
                    "--time <max_execution_time> Set the maximum execution time for the program (required, unless --accuracy is specified)\n"
                    "--accuracy                 Run tests to validate the correctness of the algorithm (optional, --time parameter is not required)\n"
                    "--query <num_tests>        Set the number of tests for reachability, default value is 100000 (optional)\n"
                    "--result_file <result_file_path>  Set the file path for result output, default is \"../output/result.csv\" (optional)\n"
                    "--result_dir <result_dir_path>    Set the directory path for query time output, default is \"../output/query_time/\" (optional)\n"
                    "--random <num_nodes> <avg_degree> Use a random DAG of a specified number of nodes and average degree\n"
                    "--file <graph_file_path>    Use the specified graph file as input\n"
                    "--algorithm <algorithm_name> [algorithm_params]  Specify the reachability algorithm to use and its parameters (required)\n"
                    "\n"
                    "Reachability Algorithms:\n"
                    "orse_toy <x> <r>           ORSE_Toy algorithm, with parameters <x> and <r>\n"
                    "bfl <K>                    BFL algorithm, with parameter <K>\n"
                    "grail <t> <ltype> <dim>    GRAIL algorithm, with parameters <t>, <ltype>, and <dim>\n"
                    "pathtree <alg_type>        PathTree algorithm, with parameter <alg_type>\n"
                    "tol <style> <opr>          TOL algorithm, with parameters <style> and <opr>\n"
                    "gripp                      GRIPP algorithm\n"
                    "ferrari <k> <seeds> <global>  Ferrari algorithm, with parameters <k>, <seeds>, and <global>\n"
                    "ip <k> <h> <mu>            IP algorithm, with parameters <k>, <h>, and <mu>\n"
                    "pll <use_RQPrunedLandmarkLabeling>  PLL algorithm, with parameter <use_RQPrunedLandmarkLabeling>\n"
                    "preach                     PReaCH algorithm\n"
                    "dbl                        DBL algorithm\n"
                    "\n"
                    "Examples:\n"
                    "- Run the reachability algorithm ORSE_Toy of specific parameters on a randomly generated DAG with 100 nodes and an average degree of 3 with a maximum execution time of 10 seconds:\n"
                    "  ./reachability --time 10 --random 100 3 --algorithm orse_toy 32 2.0\n"
                    "\n"
                    "- Run the reachability algorithm BFL of specific parameters on a specified graph file:\n"
                    "  ./reachability --time 1000 --file /path/to/graph.txt --algorithm bfl 5\n"
                    "\n"
                    "- Run tests to validate the correctness of the algorithm PLL on a random graph without limiting the maximum execution time:\n"
                    "  ./reachability --accuracy --random 1000 1 --algorithm pll 1 \n"
                    "\n"
                    "Please note that either the `--random` or `--file` option is required, and you should replace specific values and file paths with your own in the examples above.";
    std::cout << h << std::endl;
}

void algorithmUsage(const std::string& algorithm_name) {
    if (algorithm_name == "orse_toy") {
        std::cout << "Usage of orse_toy:\n"
                     "Total: 2 argument(s)\n"
                     "arg[0]: x\n"
                     "arg[1]: r\n"
                  << std::endl;
    } else if (algorithm_name == "bfl") {
        std::cout << "Usage of bfl:\n"
                     "Total: 1 argument(s)\n"
                     "arg[0]: K\n"
                  << std::endl;
    } else if (algorithm_name == "grail") {
        std::cout << "Usage of grail:\n"
                     "Total: 3 argument(s)\n"
                     "arg[0]: t\n"
                     "arg[1]: ltype\n"
                     "arg[2]: dim\n"
                  << std::endl;
    } else if (algorithm_name == "pathtree") {
        std::cout << "Usage of pathtree:\n"
                     "Total: 1 argument(s)\n"
                     "arg[0]: alg_type\n"
                  << std::endl;
    } else if (algorithm_name == "tol") {
        std::cout << "Usage of tol:\n"
                     "Total: 2 argument(s)\n"
                     "arg[0]: style\n"
                     "arg[1]: opr\n"
                  << std::endl;
    } else if (algorithm_name == "gripp") {
        std::cout << "Usage of pathtree:\n"
                     "Total: 0 argument(s)\n"
                  << std::endl;
    } else if (algorithm_name == "ferrari") {
        std::cout << "Usage of ferrari:\n"
                     "Total: 3 argument(s)\n"
                     "arg[0]: k\n"
                     "arg[1]: seeds\n"
                     "arg[2]: global\n"
                  << std::endl;
    } else if (algorithm_name == "ip") {
        std::cout << "Usage of ip:\n"
                     "Total: 3 argument(s)\n"
                     "arg[0]: k\n"
                     "arg[1]: h\n"
                     "arg[2]: mu\n"
                  << std::endl;
    } else if (algorithm_name == "pll") {
        std::cout << "Usage of pll:\n"
                     "Total: 1 argument(s)\n"
                     "arg[0]: use_RQPrunedLandmarkLabeling\n"
                  << std::endl;
    } else if (algorithm_name == "preach") {
        std::cout << "Usage of preach:\n"
                     "Total: 0 argument(s)\n"
                  << std::endl;
    } else if (algorithm_name == "dbl") {
        std::cout << "Usage of dbl:\n"
                     "Total: 0 argument(s)\n"
                  << std::endl;
    }
}

int main(int argc, char* argv[]) {
    start_time = std::chrono::high_resolution_clock::now();

    if (argc == 1) {
        usage();
        return 0;
    }
    int n = 0, d = 0;
    std::string file_path;
    bool test_accuracy = false;
    int check_reachable_times = 100000;
    std::string result_file = "../output/result.csv";
    std::string result_dir = "../output/query_time/";
    Algorithm *algorithm;

    int i = 1;
    while (i < argc) {
        if (strcmp("--help", argv[i]) == 0) {
            usage();
            return 0;
        }
        if (strcmp("--time", argv[i]) == 0) {  // timeout second
            ++i;
            if (i >= argc) {
                usage();
                return 0;
            }
            max_time_second = atoi(argv[i++]);
        } else if (strcmp("--accuracy", argv[i]) == 0) {  // test accuarcy
            test_accuracy = true;
            ++i;
        } else if (strcmp("--query", argv[i]) == 0) {  // query num
            ++i;
            if (i >= argc) {
                usage();
                return 0;
            }
            check_reachable_times = atoi(argv[i++]);
        } else if (strcmp("--result_file", argv[i]) == 0) {  // result.csv file path
            ++i;
            if (i >= argc) {
                usage();
                return 0;
            }
            result_file = argv[i++];
        } else if (strcmp("--result_dir", argv[i]) == 0) {  // query file output directory
            ++i;
            if (i >= argc) {
                usage();
                return 0;
            }
            result_dir = argv[i++];
        } else if (strcmp("--random", argv[i]) == 0) {  // random graph
            ++i;
            if (i + 1 >= argc) {
                usage();
                return 0;
            }
            n = atoi(argv[i++]);
            d = atoi(argv[i++]);
        } else if (strcmp("--file", argv[i]) == 0) {  // graph file
            ++i;
            if (i >= argc) {
                usage();
                return 0;
            }
            file_path = argv[i++];
        } else if (strcmp("--algorithm", argv[i]) == 0) {
            ++i;
            if (i >= argc) {
                usage();
                return 0;
            }
            std::string algorithm_name = argv[i++];
            if (algorithm_name == "orse_toy") {
                if (i + 2 > argc) {
                    algorithmUsage(algorithm_name);
                    return 0;
                }
                int x = atoi(argv[i++]);
                float r = atof(argv[i++]);
                algorithm = new ORSE_Toy(x, r);
            } else if (algorithm_name == "bfl") {
                if (i + 1 > argc) {
                    algorithmUsage(algorithm_name);
                    return 0;
                }
                int K = atoi(argv[i++]);
                algorithm = new BFL(K);
            } else if (algorithm_name == "grail") {
                if (i + 3 > argc) {
                    algorithmUsage(algorithm_name);
                    return 0;
                }
                int t = atoi(argv[i++]);
                int ltype = atoi(argv[i++]);
                int dim = atoi(argv[i++]);
                algorithm = new GrailWrapper(t, ltype, dim);
            } else if (algorithm_name == "pathtree") {
                if (i + 1 > argc) {
                    algorithmUsage(algorithm_name);
                    return 0;
                }
                int alg_type = atoi(argv[i++]);
                algorithm = new PathTreeWrapper(alg_type);
            } else if (algorithm_name == "tol") {
                if (i + 2 > argc) {
                    algorithmUsage(algorithm_name);
                    return 0;
                }
                int style = atoi(argv[i++]);
                int opr = atoi(argv[i++]);
                algorithm = new TOLWrapper(style, opr);
            } else if (algorithm_name == "gripp") {
                if (i > argc) {
                    algorithmUsage(algorithm_name);
                    return 0;
                }
                algorithm = new GrippWrapper();
            } else if (algorithm_name == "ferrari") {
                if (i + 3 > argc) {
                    algorithmUsage(algorithm_name);
                    return 0;
                }
                unsigned k = atoi(argv[i++]);
                unsigned seeds = atoi(argv[i++]);
                bool global = atoi(argv[i++]);
                algorithm = new Ferrari(k, seeds, global);
            } else if (algorithm_name == "ip") {
                if (i + 3 > argc) {
                    algorithmUsage(algorithm_name);
                    return 0;
                }
                int k = atoi(argv[i++]);
                int h = atoi(argv[i++]);
                int mu = atoi(argv[i++]);
                algorithm = new IP(k, h, mu);
            } else if (algorithm_name == "pll") {
                if (i + 1 > argc) {
                    algorithmUsage(algorithm_name);
                    return 0;
                }
                bool use_RQPrunedLandmarkLabeling = atoi(argv[i++]);
                algorithm = new PLL(use_RQPrunedLandmarkLabeling);
            } else if (algorithm_name == "preach") {
                if (i > argc) {
                    algorithmUsage(algorithm_name);
                    return 0;
                }
                algorithm = new PReaCH();
            } else if (algorithm_name == "dbl") {
                if (i > argc) {
                    algorithmUsage(algorithm_name);
                    return 0;
                }
                algorithm = new DBLWrapper();
            }
        }
    }

    Graph *graph;
    if (file_path.empty()) {
        graph = new Graph(n, d, "random-" + std::to_string(n) + "-" + std::to_string(d));
    } else {
        auto pos = file_path.find_last_of('/');
        auto graph_name = file_path.substr(pos + 1);
        graph_name = graph_name.substr(0, graph_name.size() - 4);
        graph = new Graph(file_path, graph_name);
    }

    if (test_accuracy) {
        algorithm->construction(*graph);
        AutoTest autoTest(graph, algorithm);
        auto ret = autoTest.checkCorrectness();
        if (ret.first) {
            std::cout << "Correctness test passed." << std::endl;
        } else {
            std::cout << "Correctness test failed on test " << ret.second.second << "to" << ret.second.second << "." << std::endl;
        }
        algorithm->reset();
        delete algorithm;
        delete graph;
        return 0;
    }

    auto profile = testAlgorithmsOnGraph(*graph, algorithm, check_reachable_times);

    std::ofstream myfile;
    myfile.open(result_file, std::ios_base::app);
    myfile << profile.algorithm_name << ","
           << profile.graph_name << ","
           << profile.params << ","
           << profile.preparation_time_ns << ","
           << profile.index_size << ","
           << profile.query_num << ","
           << profile.average_has_path_time_ns << "\n";
    myfile.close();

    std::string query_file_name = result_dir + "/" + profile.algorithm_name + "_" + profile.params + "_" + profile.graph_name + ".txt";
    myfile.open(query_file_name);
    for (const auto &x : profile.has_path_times_ns) {
        myfile << x << '\n';
    }
    myfile.close();

    algorithm->reset();
    delete algorithm;
    delete graph;

    return 0;
}

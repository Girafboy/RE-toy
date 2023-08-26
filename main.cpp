#include <iomanip>
#include <iostream>
#include <ostream>
#include <queue>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <cstring>
#include <numeric>

#include "Graph.h"
#include "AutoTest.h"
#include "Profile.h"

#include "algorithms/Kang.h"
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

    std::vector<long long> has_path_times_ns;
    AutoTest auto_test = AutoTest(&graph, algorithm);
    auto queries = auto_test.generateQueries(check_reachable_times, check_only_reached);
    for (const auto &query : queries) {
        time1 = std::chrono::high_resolution_clock::now();
        algorithm->TC_haspath(query.first, query.second);
        time2 = std::chrono::high_resolution_clock::now();
        ms_nano = time2 - time1;
        has_path_times_ns.push_back(ms_nano.count());
        ms_nano = time2 - start_time;
        if ((unsigned long long)max_time_second * 1000000000 - ms_nano.count() < 2000000000) {
            break;
        }
    }
    profile.query_num = has_path_times_ns.size();

    long long sum = std::accumulate(has_path_times_ns.begin(), has_path_times_ns.end(), 0ll);
    double mean = (double)sum / (double)has_path_times_ns.size();
    profile.average_has_path_time_ns = mean;

    std::vector<double> diff(has_path_times_ns.size());
    std::transform(has_path_times_ns.begin(), has_path_times_ns.end(), diff.begin(), [mean](double x) { return x - mean; });
    double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
    double stdev = std::sqrt(sq_sum / (double)has_path_times_ns.size());
    profile.standard_deviation_of_has_path_times_ns = stdev;

    profile.index_size = algorithm->getIndexSize();

    return profile;
}

//void main_test() {
//    std::vector<Algorithm *> algorithms = {
//        /****************************
//         * Kang
//         ****************************/
//        new Kang(32),
//        new Kang(64),
//        new Kang(128),
//        new Kang(256),
////        new Kang(512),
////        new Kang(1024),
////        new Kang(2048),
////        new Kang(4096),
//        // new Kang(1024*16),
//        /****************************
//         * BFL
//         ****************************/
////        new BFL(1),
////        new BFL(2),
////        new BFL(5),
////        new BFL(10),
////        new BFL(20),
////        new BFL(50),
////        new BFL(100),
////        new BFL(200),
////        new BFL(500),
//        /****************************
//         * Grail
//         ****************************/
////        new GrailWrapper(1, 0, 2),
////        new GrailWrapper(1, 0, 3),
////        new GrailWrapper(1, 0, 4),
////        new GrailWrapper(1, 0, 5),
////        new GrailWrapper(1, 1, 2),
////        new GrailWrapper(1, 1, 3),
////        new GrailWrapper(1, 1, 4),
////        new GrailWrapper(1, 1, 5),
////        new GrailWrapper(-2, 0, 2),
////        new GrailWrapper(-2, 0, 3),
////        new GrailWrapper(-2, 0, 4),
////        new GrailWrapper(-2, 0, 5),
////        new GrailWrapper(-2, 1, 2),
////        new GrailWrapper(-2, 1, 3),
////        new GrailWrapper(-2, 1, 4),
////        new GrailWrapper(-2, 1, 5),
//        /****************************
//         * Path Tree
//         ****************************/
////        new PathTreeWrapper(1),
////        new PathTreeWrapper(2),
////        new PathTreeWrapper(3),
////        new PathTreeWrapper(4),
//        /****************************
//         * TOL
//         ****************************/
////        new TOLWrapper(0, 1),
////        new TOLWrapper(0, 2),
////        new TOLWrapper(1, 1),  // todo: 正确性有问题
////        new TOLWrapper(1, 2),
//        /****************************
//         * Gripp
//         ****************************/
////        new GrippWrapper(),  // 极慢
//        /****************************
//         * buTR
//            TODO: buTR的实现有问题
//         ****************************/
//        // new buTRWrapper(),
//        /****************************
//         * Ferrari
//         ****************************/
////        new Ferrari(2, 32, true),
//        /****************************
//         * IP
//            TODO: BUG
//         ****************************/
////         new IP(2, 2),
//        /****************************
//         * PLL
//         ****************************/
////        new PLL(true),
////        new PLL(false),
//        /****************************
//         * TF-label
//            TODO: 有bug，随机性强，偶尔跑通也慢，不知道是不是实现问题
//            Algorithm TOL on Graph random-100-10 has a preparation time of 0.995833 ms, an total 'has_path' time of 3.10062 ms, and an index space of 10060 B.
//            Algorithm TF-label on Graph random-100-10 has a preparation time of 4.97579 ms, an total 'has_path' time of 52.8992 ms, and an index space of 18360 B.
//         ****************************/
//        // new TFLabel(),
//        // new TFL(),
//        /****************************
//         * PReaCH
//         ****************************/
////        new PReaCH(),
//        /****************************
//         * DBL
//         ****************************/
////        new DBLWrapper(),
//    };
//
//    /***************************************** Correctness Test *****************************************/
//    std::vector<Graph> graphs_for_correctness_test = {
//            Graph(5, 1, "random-5-1"),
//            Graph(100, 10, "random-100-10"),
//    };
//    for (auto algorithm: algorithms) {
//        for (const auto &graph : graphs_for_correctness_test) {
//            algorithm->construction(graph);
//            AutoTest autoTest(&graph, algorithm);
//            assert(autoTest.checkCorrectness().first);
//            algorithm->reset();
//        }
//        std::cout << "Correctness Test Pass! (" << algorithm->getName() << ", " << algorithm->getParams() << ")" << std::endl;
//    }
//    /***************************************** Correctness Test *****************************************/
//
//    /***************************************** Perfomance Test *****************************************/
//    Profile profile;
//
//    int check_reachable_times = 100000;
//
//    std::vector<Graph> graphs = {
//        // Graph(100, 10, "random-100-10"),
//        Graph(1000, 1, "random-1000-1"),
////        Graph(1000, 2, "random-1000-2"),
////        Graph(1000, 5, "random-1000-5"),
////        Graph(1000, 10, "random-1000-10"),
////        Graph(1000, 20, "random-1000-20"),
////        Graph(1000, 50, "random-1000-50"),
//        Graph(1000, 100, "random-1000-100"),
//        // Graph(10000, 10, "random-10000-10"),
//        // Graph(100000, 10, "random-100000-10"),
//        // Graph("../graphs/large_real/citeseer_scc.txt", "citeseer_scc"),
//        // Graph("../graphs/small_real_dense/arXiv_sub_6000-1.txt", "arXiv_sub_6000-1")
//    };
//
//
//    std::ofstream myfile;
//    myfile.open("../output/result.csv");
//    myfile << "algorithm,graph,params,construction(ms),index(B),query(us)\n";
//    std::cout << std::setw(20) << "Algorithm" << std::setw(20) << "Graph" << std::setw(50) << "Params" << std::setw(20) << "Construction(ms)" << std::setw(20) << "Index(B)" << std::setw(20) << "Query(us)" << std::endl;
//    for (const auto &graph: graphs) {
//        for (auto algorithm: algorithms) {
//            profile = testAlgorithmsOnGraph(graph, algorithm, check_reachable_times);
//
//            myfile << profile.algorithm_name << ","
//                << profile.graph_name << ","
//                << profile.params << ","
//                << profile.preparation_time << ","
//                << profile.index_size << ","
//                << profile.total_has_path_time * 1000 / check_reachable_times << "\n";
//            std::cout << std::setw(20) << profile.algorithm_name
//                << std::setw(20) << profile.graph_name
//                << std::setw(50) << profile.params
//                << std::setw(20) << profile.preparation_time
//                << std::setw(20) << profile.index_size
//                << std::setw(20) << profile.total_has_path_time * 1000 / check_reachable_times << std::endl;
//        }
//    }
//    myfile.close();
//    /***************************************** Perfomance Test *****************************************/
//
//    for (auto algorithm: algorithms) {
//        delete algorithm;
//    }
//}

void usage() {
    std::cout << "Usage:\n"
                 "tc [--help] todo\n"
                 "Description:\n"
                 "\t --help \t\t  print the help message\n"
                 "\t todo\n"
              << std::endl;
}

void algorithmUsage(const std::string& algorithm_name) {
    if (algorithm_name == "xjump") {
        std::cout << "Usage of xjump:\n"
                     "Total: 1 argument(s)\n"
                     "arg[0]: x\n"
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
    Algorithm *algorithm;

    int i = 1;
    while (i < argc) {
        if (strcmp("--help", argv[i]) == 0) {
            usage();
            return 0;
        }
        if (strcmp("--time", argv[i]) == 0) {
            ++i;
            if (i >= argc) {
                usage();
                return 0;
            }
            max_time_second = atoi(argv[i++]);
        } else if (strcmp("--accuracy", argv[i]) == 0) {
            test_accuracy = true;
            ++i;
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
            if (algorithm_name == "xjump") {
                if (i + 1 > argc) {
                    algorithmUsage(algorithm_name);
                    return 0;
                }
                int x = atoi(argv[i++]);
                algorithm = new Kang(x);
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
            // todo: algorithms
        }
    }

    Graph *graph;
    if (file_path.empty()) {
        graph = new Graph(n, d, "random-" + std::to_string(n) + "-" + std::to_string(d));
    } else {
        auto pos = file_path.find_last_of('/');
        auto graph_name = file_path.substr(pos + 1);
        graph = new Graph(file_path, graph_name);
    }

    if (test_accuracy) {
        // todo: test accuracy
        std::cout << "Not implemented yet." << std::endl;
    }

    int check_reachable_times = 100000;
    auto profile = testAlgorithmsOnGraph(*graph, algorithm, check_reachable_times);

    std::ofstream myfile;
    myfile.open("../output/result.csv", std::ios_base::app);
    myfile << profile.algorithm_name << ","
           << profile.graph_name << ","
           << profile.params << ","
           << profile.preparation_time_ns << ","
           << profile.index_size << ","
           << profile.query_num << ","
           << profile.average_has_path_time_ns << ","
           << profile.standard_deviation_of_has_path_times_ns << "\n";
    myfile.close();

    algorithm->reset();
    delete algorithm;
    delete graph;

    return 0;
}

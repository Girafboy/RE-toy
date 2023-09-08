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

void testAccuracy() {
    IP ip(2, 2, 100);
    int n = 100, d = 10;
    Graph graph(n, d, "random");
    ip.construction(graph);
    AutoTest autoTest(&graph, &ip);
    auto ret = autoTest.checkCorrectness();
    if (ret.first) {
        std::cout << "Correctness test passed." << std::endl;
    } else {
        std::cout << "Correctness test failed." << std::endl;
    }
}

Profile testAlgorithmsOnGraph(const Graph &graph, Algorithm *algorithm, int check_reachable_times, int sample_num, bool check_only_reached=false) {
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

    auto left = has_path_times_ns.begin();
    for (int i = 1; i < sample_num - 1; ++i) {
        auto mid = left + has_path_times_ns.size() / (sample_num - 1);
        std::nth_element(left, mid, has_path_times_ns.end());
        if (i == 1) {
            profile.has_path_time_samples.push_back(*min_element(has_path_times_ns.begin(), mid + 1));
        }
        profile.has_path_time_samples.push_back(*mid);
        left = mid;
    }
    profile.has_path_time_samples.push_back(*max_element(left, has_path_times_ns.end()));

    return profile;
}


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
    int sample_num = 5;
    auto profile = testAlgorithmsOnGraph(*graph, algorithm, check_reachable_times, sample_num);

    std::stringstream ss;
    ss << '[';
    for (const auto &t : profile.has_path_time_samples) {
        ss << t << "; ";
    }
    ss << ']';

    std::ofstream myfile;
    myfile.open("../output/result.csv", std::ios_base::app);
    myfile << profile.algorithm_name << ","
           << profile.graph_name << ","
           << profile.params << ","
           << profile.preparation_time_ns << ","
           << profile.index_size << ","
           << profile.query_num << ","
           << profile.average_has_path_time_ns << ","
           << ss.str() << "\n";
    myfile.close();

    algorithm->reset();
    delete algorithm;
    delete graph;

    return 0;
}

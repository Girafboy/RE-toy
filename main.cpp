#include <iostream>
#include <ostream>
#include <fstream>
#include <cstring>

#include "Graph.h"
#include "AutoTest.h"
#include "Profile.h"
#include "Timer.h"

#include "algorithms/RE_Toy.h"
#include "algorithms/BFL.h"
#include "algorithms/GrailWrapper.h"
#include "algorithms/PathTreeWrapper.h"
#include "algorithms/TOLWrapper.h"
#include "algorithms/GrippWrapper.h"
#include "algorithms/Ferrari.h"
#include "algorithms/IP.h"
#include "algorithms/PLL.h"
#include "algorithms/PReaCH.h"
#include "algorithms/DBLWrapper.h"

using re::RE_Toy;
using bfl::BFL;
using grail::GrailWrapper;
using path_tree::PathTreeWrapper;
using tol::TOLWrapper;
using gripp::GrippWrapper;
using ferrari::Ferrari;
using ip::IP;
using pll::PLL;
using preach::PReaCH;
using dbl::DBLWrapper;

Timer timer;

void usage();

void algorithmUsage(const std::string &algorithm_name);

int main(int argc, char *argv[]) {
    if (argc == 1) {
        usage();
        return 0;
    }

    // arguments read from command line
    int n = 0;
    int d = -1;
    int seed = 0;
    std::string file_path;
    bool test_accuracy = false;
    int check_reachable_times = 100000;
    std::string result_file;
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
            timer.setMaxTimeSecond(atoi(argv[i++]));
        } else if (strcmp("--accuracy", argv[i]) == 0) {  // test accuarcy
            test_accuracy = true;
            ++i;
        } else if (strcmp("--query_num", argv[i]) == 0) {  // query num
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
        } else if (strcmp("--graph", argv[i]) == 0) {  // input graph
            ++i;
            if (i >= argc) {
                usage();
                return 0;
            }
            std::string graph_type = argv[i++];
            if (graph_type == "--random") {  // random DAG
                if (i + 1 >= argc) {
                    usage();
                    return 0;
                }
                n = atoi(argv[i++]);
                d = atoi(argv[i++]);
            } else if (graph_type == "--complete") {  // complete DAG
                if (i >= argc) {
                    usage();
                    return 0;
                }
                n = atoi(argv[i++]);
            } else if (graph_type == "--file") {  // graph from file
                if (i >= argc) {
                    usage();
                    return 0;
                }
                file_path = argv[i++];
            }
        } else if (strcmp("--seed", argv[i]) == 0) {
            ++i;
            if (i >= argc) {
                usage();
                return 0;
            }
            seed = atoi(argv[i++]);
        } else if (strcmp("--algorithm", argv[i]) == 0) {  // algorithm
            ++i;
            if (i >= argc) {
                usage();
                return 0;
            }
            std::string algorithm_name = argv[i++];
            if (algorithm_name == "re_toy") {
                if (i + 2 > argc) {
                    algorithmUsage(algorithm_name);
                    return 0;
                }
                int x = atoi(argv[i++]);
                float delta = atof(argv[i++]);
                algorithm = new RE_Toy(x, delta);
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
            } else {
                std::cout << "Invalid algorithm name." << std::endl;
                usage();
                return 0;
            }
        }
    }

    // prepare graph
    Graph *graph;
    if (!file_path.empty()) {  // read graph from file
        auto pos = file_path.find_last_of('/');
        auto graph_name = file_path.substr(pos + 1);
        graph_name = graph_name.substr(0, graph_name.size() - 4);
        graph = new Graph(file_path, graph_name);
    } else if (d < 0) {  // generate complete graph
        graph = new Graph(n, "complete-" + std::to_string(n));
    } else {  // generate random graph
        graph = new Graph(n, d, seed, "random-" + std::to_string(n) + "-" + std::to_string(d));
    }

    // test algorithm on graph
    AutoTest autoTest(graph, algorithm);
    if (test_accuracy) {  // test the accuracy of the algorithm on the graph
        algorithm->construction(*graph);
        auto ret = autoTest.checkCorrectness();
        if (ret.first) {
            std::cout << "Correctness test passed (" << algorithm->getName() << ", " << algorithm->getParams() << ", "
                      << graph->getName() << ")" << std::endl;
        } else {
            std::cout << "Correctness test failed! (" << algorithm->getName() << ", " << algorithm->getParams() << ", "
                      << graph->getName() << ", from " << ret.second.first << " to " << ret.second.second << ")"
                      << std::endl;
        }
    } else {  // test index size and query time of the algorithm on the graph
        auto profile = autoTest.testAlgorithmOnGraph(check_reachable_times);
        if (!result_file.empty()) {
            std::ofstream myfile;
            myfile.open(result_file, std::ios_base::app);
            myfile << profile.algorithm_name << ","
                   << profile.graph_name << ","
                   << profile.params << ","
                   << profile.index_size << ","
                   << profile.query_num << ","
                   << profile.average_has_path_time_ns << "\n";
            myfile.close();
        }
    }

    algorithm->reset();
    delete algorithm;
    delete graph;

    return 0;
}

void usage() {
    std::string h = "reachability - A program for running reachability algorithms on a given directed graph\n"
                    "\n"
                    "Usage:\n"
                    "./reachability [options]\n"
                    "\n"
                    "Options:\n"
                    "--help                     Show help message\n"
                    "--time <seconds>           Set the maximum execution time in seconds for the program (required, unless --accuracy is specified)\n"
                    "--accuracy                 Run tests to validate the correctness of the algorithm (optional, --time parameter is not required)\n"
                    "--query_num <number>       Set number of queries, default value is 100000 (optional)\n"
                    "--result_file <file>       Set the file path for result output (optional, use when generating the result file)\n"
                    "--graph <type>             Specify input graph type (required)\n"
                    "   --random <n> <d>        Generate a random DAG with <n> nodes and <d> average degree\n"
                    "   --complete <n>          Generate a complete DAG with <n> nodes\n"
                    "   --file <file_path>      Load a directed graph from file and convert it to a DAG\n"
                    "--seed <seed>              Set the seed for the generation of the random graph.\n"
                    "--algorithm <algorithm_name> [algorithm_params]  Specify the reachability algorithm to use and its parameters (required)\n"
                    "\n"
                    "Reachability Algorithms:\n"
                    "re_toy <x> <delta>         RE-toy algorithm\n"
                    "bfl <K>                    BFL algorithm\n"
                    "grail <t> <ltype> <dim>    GRAIL algorithm\n"
                    "pathtree <alg_type>        PathTree algorithm\n"
                    "tol <style> <opr>          TOL algorithm\n"
                    "gripp                      GRIPP algorithm\n"
                    "ferrari <k> <seeds> <global>  Ferrari algorithm\n"
                    "ip <k> <h> <mu>            IP algorithm\n"
                    "pll <use_RQPrunedLandmarkLabeling>  PLL algorithm\n"
                    "preach                     PReaCH algorithm\n"
                    "dbl                        DBL algorithm\n"
                    "\n"
                    "Examples:\n"
                    "- Run the reachability algorithm RE-toy of specific parameters on a complete DAG with 1000 nodes with a maximum execution time of 100 seconds, and store the results in the specified path:\n"
                    "  ./reachability --time 100 --graph --complete 1000 --algorithm re 32 2.0 --result_file ./result.csv\n"
                    "\n"
                    "- Run tests to validate the correctness of the algorithm PLL on a randomly generated DAG with 100 nodes and an average degree of 3 with a specific seed without limiting the maximum execution time:\n"
                    "  ./reachability --accuracy --graph --random 100 3 --seed 29 --algorithm pll 1 \n"
                    "\n"
                    "Please note that you should replace specific values and file paths with your own in the examples above.";
    std::cout << h << std::endl;
}

void algorithmUsage(const std::string &algorithm_name) {
    if (algorithm_name == "re_toy") {
        std::cout << "Usage of re_toy:\n"
                     "Total: 2 argument(s)\n"
                     "arg[0]: x\n"
                     "arg[1]: delta\n"
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

#ifndef TC_AUTOTEST_H
#define TC_AUTOTEST_H


#include "algorithms/Algorithm.h"
#include "Graph.h"
#include "Profile.h"


class AutoTest {
private:
    Algorithm *const algorithm_ptr;
    const Graph *const graph_ptr;

public:
    explicit AutoTest(const Graph *graph_ptr, Algorithm *algorithm_ptr);

    std::pair<bool, std::pair<int, int>> checkCorrectness();

    std::vector<std::pair<int, int>> generateQueries(int m, bool check_only_reached = false);

    Profile testAlgorithmOnGraph(int check_reachable_times, bool check_only_reached = false);
};


#endif

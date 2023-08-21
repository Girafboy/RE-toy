#ifndef TC_AUTOTEST_H
#define TC_AUTOTEST_H


#include "algorithms/Algorithm.h"
#include "Graph.h"


class AutoTest {
private:
    Algorithm *algorithm_ptr;
    const Graph *graph_ptr;
    std::vector<std::pair<int, int>> queries;

public:
    explicit AutoTest(const Graph *graph_ptr, Algorithm *algorithm_ptr);

    std::pair<bool, std::pair<int, int>> checkCorrectness();

    void generateQueries(int m, bool check_only_reached = false);

    void runQueryTest();
};


#endif

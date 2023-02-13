//
// Created by 王星力 on 2022/11/8.
//

#ifndef KANG_AUTOTEST_H
#define KANG_AUTOTEST_H


#include "algorithms/Algorithm.h"
#include "Graph.h"


class AutoTest {
private:
    Algorithm *algorithm_ptr;
    const Graph *graph_ptr;

    std::pair<bool, std::pair<int, int>> checkCorrectness();
    std::vector<std::pair<int, int>> getReachableQueries(int m);

public:
    explicit AutoTest(const Graph *graph_ptr, Algorithm *algorithm_ptr);
    double runAutoTest(int m = 100, bool check_correctness = true, bool check_only_reached = false);
    long long getIndexSize();
};


#endif //KANG_AUTOTEST_H

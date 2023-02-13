//
// Created by 王星力 on 2022/11/8.
//

#include <queue>
#include <chrono>
#include <iostream>
#include <random>
#include <utility>
#include <cassert>

#include "AutoTest.h"

AutoTest::AutoTest(const Graph *graph_ptr, Algorithm *algorithm_ptr) : algorithm_ptr(algorithm_ptr), graph_ptr(graph_ptr) {

}

std::pair<bool, std::pair<int, int>> AutoTest::checkCorrectness() {
    auto n = graph_ptr->size();
    for (int i = 0; i < n; ++i) {
        std::vector<bool> visited(n);
        std::queue<int> q;
        visited[i] = true;
        q.push(i);
        while (!q.empty()) {
            int tmp = q.front();
            q.pop();
            for (const auto &j : graph_ptr->getSuccessors(tmp)) {
                if (!visited[j]) {
                    visited[j] = true;
                    q.push(j);
                }
            }
        }
        for (int j = 0; j < n; ++j) {
            if (algorithm_ptr->TC_haspath(i, j) != visited[j]) {
//                std::cout << i << " to " << j << " in Graph " << graph_ptr->getName() << " failed!" << std::endl;
                return std::make_pair(false, std::make_pair(i, j));
            }
        }
    }
    return std::make_pair(true, std::make_pair(0, 0));
}

double AutoTest::runAutoTest(int m, bool check_correctness, bool check_only_reached) {
    if (check_correctness) {
        auto tmp = checkCorrectness();
        assert(tmp.first);
    }
    int n = (int)graph_ptr->size();
//    m = std::min(m, n * n);
    std::uniform_int_distribution<int> u(0, n - 1);
    std::default_random_engine e;
    e.seed(std::chrono::system_clock::now().time_since_epoch().count());
    std::vector<std::pair<int, int>> queries;
    if (!check_only_reached) {
        for (int i = 0; i < m; ++i) {
            int r1 = u(e);
            int r2 = u(e);
            queries.emplace_back(r1, r2);
        }
    } else {
        queries = getReachableQueries(m);
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    for (const auto &query: queries) {
        algorithm_ptr->TC_haspath(query.first, query.second);
    }
//    for (size_t i = 0; i < queries.size(); ++i) {
//        auto query = queries[i];
//        if (i % 1 == 0) {
//            std::cout << "i = " << i;
//            std::cout << ", source = " << query.first << ", target = " << query.second << std::endl;
//        }
//        algorithm_ptr->TC_haspath(query.first, query.second);
//    }
    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms_double = t2 - t1;
    return ms_double.count();
}

std::vector<std::pair<int, int>> AutoTest::getReachableQueries(int m) {
    std::vector<std::pair<int, int>> queries;
    auto n = graph_ptr->size();
    for (int i = 0; i < n; ++i) {
        std::vector<bool> visited(n);
        std::queue<int> q;
        visited[i] = true;
        q.push(i);
        while (!q.empty()) {
            int tmp = q.front();
            q.pop();
            for (const auto &j : graph_ptr->getSuccessors(tmp)) {
                if (!visited[j]) {
                    visited[j] = true;
                    q.push(j);
                }
            }
        }
        for (int j = 0; j < n; ++j) {
            if (visited[j]) {
                queries.emplace_back(i, j);  // todo: 无随机性，都是挨在一起的
            }
            if (queries.size() == m) {
                return queries;
            }
        }
    }
    return queries;
}

long long AutoTest::getIndexSize() {
    return algorithm_ptr->getIndexSize();
}

#include <queue>
#include <chrono>
#include <iostream>
#include <random>
#include <utility>

#include "AutoTest.h"

AutoTest::AutoTest(const Graph *graph_ptr, Algorithm *algorithm_ptr) : algorithm_ptr(algorithm_ptr),
                                                                       graph_ptr(graph_ptr) {

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
            for (const auto &j: graph_ptr->getSuccessors(tmp)) {
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

bool AutoTest::runQueryTest(const std::pair<int, int> &query) {
    return algorithm_ptr->TC_haspath(query.first, query.second);
}

std::vector<std::pair<int, int>> AutoTest::generateQueries(int m, bool check_only_reached) {
    std::vector<std::pair<int, int>> queries;
    int n = (int)graph_ptr->size();
    if (check_only_reached) {
        for (int i = 0; i < n; ++i) {
            std::vector<bool> visited(n);
            std::queue<int> q;
            visited[i] = true;
            q.push(i);
            while (!q.empty()) {
                int tmp = q.front();
                q.pop();
                for (const auto &j: graph_ptr->getSuccessors(tmp)) {
                    if (!visited[j]) {
                        visited[j] = true;
                        q.push(j);
                    }
                }
            }
            for (int j = 0; j < n; ++j) {
                if (visited[j]) {
                    queries.emplace_back(i, j);
                }
                if (queries.size() == m) {
                    return queries;
                }
            }
        }
    } else {
        std::uniform_int_distribution<int> u(0, (int)graph_ptr->number_of_original_nodes() - 1);
        std::default_random_engine e;
        e.seed(std::chrono::system_clock::now().time_since_epoch().count());
        for (int i = 0; i < m; ++i) {
            int r1 = u(e);
            int r2 = u(e);
            queries.emplace_back(graph_ptr->getSccId(r1), graph_ptr->getSccId(r2));
        }
    }
    return queries;
}

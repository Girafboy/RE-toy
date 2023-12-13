#include <queue>
#include <chrono>
#include <iostream>
#include <random>
#include <utility>
#include <algorithm>
#include <vector>
#include <numeric>

#include "AutoTest.h"

extern std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
extern long long max_time_second;

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
                return std::make_pair(false, std::make_pair(i, j));
            }
        }
    }
    return std::make_pair(true, std::make_pair(0, 0));
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
//        e.seed(std::chrono::system_clock::now().time_since_epoch().count());
        for (int i = 0; i < m; ++i) {
            int r1 = u(e);
            int r2 = u(e);
            queries.emplace_back(graph_ptr->getSccId(r1), graph_ptr->getSccId(r2));
        }
    }
    return queries;
}

Profile AutoTest::testAlgorithmOnGraph(int check_reachable_times, bool check_only_reached) {
    Profile profile;
    profile.graph_name = graph_ptr->getName();
    profile.algorithm_name = algorithm_ptr->getName();
    profile.params = algorithm_ptr->getParams();

    decltype(std::chrono::high_resolution_clock::now()) time1, time2;
    std::chrono::duration<long long, std::nano> ms_nano{};

    time1 = std::chrono::high_resolution_clock::now();
    algorithm_ptr->construction(*graph_ptr);
    time2 = std::chrono::high_resolution_clock::now();
    ms_nano = time2 - time1;
    profile.preparation_time_ns = ms_nano.count();

    profile.index_size = algorithm_ptr->getIndexSize();

    std::vector<long long> has_path_times_ns;
    auto queries = generateQueries(check_reachable_times, check_only_reached);
    for (const auto &query : queries) {
        std::vector<long long> tmp_has_path_times_ns;
        for (int _ = 0; _ < 5; ++_) {
            time1 = std::chrono::high_resolution_clock::now();
            algorithm_ptr->TC_haspath(query.first, query.second);
            time2 = std::chrono::high_resolution_clock::now();
            ms_nano = time2 - time1;
            tmp_has_path_times_ns.push_back(ms_nano.count());
        }
        std::nth_element(tmp_has_path_times_ns.begin(), tmp_has_path_times_ns.begin() + tmp_has_path_times_ns.size() / 2, tmp_has_path_times_ns.end());
        has_path_times_ns.push_back(tmp_has_path_times_ns[tmp_has_path_times_ns.size() / 2]);
        ms_nano = time2 - start_time;
        if (max_time_second * 1000000000 - ms_nano.count() < 2000000000) {
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

    profile.has_path_times_ns = std::move(has_path_times_ns);

    return profile;
}

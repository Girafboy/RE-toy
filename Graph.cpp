#include "Graph.h"

#include <cassert>
#include <cmath>
#include <utility>
#include <fstream>
#include <sstream>
#include <random>
#include <unordered_set>
#include <stack>
#include <algorithm>

Graph::Graph(const std::vector<std::vector<int>>& original_adj_list, std::string name) : name(std::move(name)) {
    mergeScc(original_adj_list);
    generateReverseAdjList();
    n_node = adj_list.size();
    n_edge = 0;
    for (const auto& list : adj_list) {
        n_edge += list.size();
    }
}

size_t Graph::getInDegree(int i) const {
    return reverse_adj_list[i].size();
}

size_t Graph::getOutDegree(int i) const {
    return adj_list[i].size();
}

Graph::Graph(const std::string &input_file_path, std::string name, bool is_dag) : name(std::move(name)) {
    std::vector<std::vector<int>> original_adj_list;
    std::ifstream graph_file;
    graph_file.open(input_file_path);
    std::string line;
    while (std::getline(graph_file, line)) {
        std::istringstream iss(line);
        int node = 0;
        iss >> node;
        assert(node == original_adj_list.size() && "Error: Nodes in the graph file are not labeled as continuous natural numbers.");
        node = iss.get();
        assert(node == ':' && "Error: Miss colon.");
        original_adj_list.emplace_back();
        while (iss >> node) {
            original_adj_list.back().push_back(node);
        }
    }
    graph_file.close();

    if (is_dag) {
        adj_list = std::move(original_adj_list);
        scc_map.resize(adj_list.size());
        std::iota(scc_map.begin(), scc_map.end(), 0);
    } else {
        mergeScc(original_adj_list);
    }

    generateReverseAdjList();

    n_node = adj_list.size();
    n_edge = 0;
    for (const auto& list : adj_list) {
        n_edge += list.size();
    }
}

Graph::Graph(int n, int d, int seed, std::string name) : name(std::move(name)) {
    int m = n * d / 2;
    adj_list.resize(n);
    std::vector<std::unordered_set<int>> adj_hash(n);
    std::uniform_int_distribution<int> u(0, n - 1);
    std::default_random_engine e(seed);
    if (d < std::sqrt(n)) {
        while (m > 0) {
            int r1 = u(e);
            int r2 = u(e);
            if (r1 == r2) {
                continue;
            }
            int v1 = std::min(r1, r2);
            int v2 = std::max(r1, r2);
            if (adj_hash[v1].find(v2) == adj_hash[v1].end()) {
                adj_list[v1].push_back(v2);
                adj_hash[v1].insert(v2);
                --m;
            }
        }
    } else {
        for (int v1 = 0; v1 < n; ++v1) {
            for (int v2 = v1+1; v2 < n; ++v2) {
                if (u(e) < d) {
                    adj_list[v1].push_back(v2);
                }
            }
        }
    }
    generateReverseAdjList();

    n_node = adj_list.size();
    n_edge = 0;
    for (const auto& list : adj_list) {
        n_edge += list.size();
    }
    scc_map.resize(n);
    std::iota(scc_map.begin(), scc_map.end(), 0);
}

Graph::Graph(int n, std::string name) : name(std::move(name)) {
    adj_list.resize(n);
    for (int u = 0; u < n; ++u) {
        for (int v = 0; v < u; ++v) {
            adj_list[u].push_back(v);
        }
    }
    generateReverseAdjList();
    n_node = n;
    n_edge = n * (n - 1) / 2;
    scc_map.resize(n);
    std::iota(scc_map.begin(), scc_map.end(), 0);
}

void Graph::generateReverseAdjList() {
    reverse_adj_list.resize(adj_list.size());
    for (int i = 0; i < adj_list.size(); ++i) {
        for (const auto &tail: adj_list[i]) {
            reverse_adj_list[tail].push_back(i);
        }
    }
}

size_t Graph::size() const {
    return n_node;
}

size_t Graph::number_of_edges() const {
    return n_edge;
}

size_t Graph::number_of_original_nodes() const {
    return scc_map.size();
}

std::vector<int> Graph::getPredecessors(int i) const {
    return reverse_adj_list[i];
}

std::vector<int> Graph::getSuccessors(int i) const {
    return adj_list[i];
}

std::string Graph::getName() const {
    return name;
}

int Graph::getSccId(int i) const {
    return scc_map[i];
}

void Graph::mergeScc(const std::vector<std::vector<int>> &original_adj_list) {
    auto n = original_adj_list.size();
    scc_map.resize(n, -1);
    int scc_count = 0;
    std::vector<int> preorder(n, -1);
    std::vector<int> lowlink(n, -1);
    std::stack<int> scc_queue;
    int i = 0;  // Preorder counter
    for (int source = 0; source < n; ++source) {
        if (scc_map[source] < 0) {
            std::stack<int> queue;
            queue.push(source);
            while (!queue.empty()) {
                auto v = queue.top();
                if (preorder[v] < 0) {
                    ++i;
                    preorder[v] = i;
                }
                bool done = true;
                for (const auto &w : original_adj_list[v]) {
                    if (preorder[w] < 0) {
                        queue.push(w);
                        done = false;
                        break;
                    }
                }
                if (done) {
                    lowlink[v] = preorder[v];
                    for (const auto &w : original_adj_list[v]) {
                        if (scc_map[w] < 0) {
                            if (preorder[w] > preorder[v]) {
                                lowlink[v] = std::min(lowlink[v], lowlink[w]);
                            } else {
                                lowlink[v] = std::min(lowlink[v], preorder[w]);
                            }
                        }
                    }
                    queue.pop();
                    if (lowlink[v] == preorder[v]) {
                        scc_map[v] = scc_count;
                        while (!scc_queue.empty() && preorder[scc_queue.top()] > preorder[v]) {
                            auto k = scc_queue.top();
                            scc_queue.pop();
                            scc_map[k] = scc_count;
                        }
                        ++scc_count;
                    } else {
                        scc_queue.push(v);
                    }
                }
            }
        }
    }

    adj_list.resize(scc_count);
    for (int u = 0; u < n; ++u) {
        for (const auto &v : original_adj_list[u]) {
            if (scc_map[u] != scc_map[v]) {
                adj_list[scc_map[u]].push_back(scc_map[v]);
            }
        }
    }

    for (int j = 0; j < scc_count; ++j) {
        std::sort(adj_list[j].begin(), adj_list[j].end());
        adj_list[j].erase(std::unique(adj_list[j].begin(), adj_list[j].end()), adj_list[j].end());
    }
}

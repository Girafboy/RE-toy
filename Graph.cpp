#include "Graph.h"

#include <cassert>
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

Graph::Graph(const std::string &input_file_path, std::string name) : name(std::move(name)) {
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

    mergeScc(original_adj_list);

    generateReverseAdjList();

    n_node = adj_list.size();
    n_edge = 0;
    for (const auto& list : adj_list) {
        n_edge += list.size();
    }
}

Graph::Graph(int n, int d, std::string name) : name(std::move(name)) {
    int m = n * d;
    adj_list.resize(n);
    std::vector<std::unordered_set<int>> adj_hash(n);
    std::uniform_int_distribution<int> u(0, n - 1);
    std::default_random_engine e;
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
    generateReverseAdjList();

    n_node = adj_list.size();
    n_edge = 0;
    for (const auto& list : adj_list) {
        n_edge += list.size();
    }
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
    scc_map.resize(n);
    int id = 0;
    int scc_count = 0;
    std::vector<int> ids(n, -1);
    std::vector<int> low_links(n);
    std::vector<bool> on_stack(n);
    std::stack<int> st;

    std::function<void(int)> dfs = [&](int u) {
        st.push(u);
        on_stack[u] = true;
        ids[u] = id;
        low_links[u] = id;
        ++id;
        for (const auto &v : original_adj_list[u]) {
            if (ids[v] == -1) {
                dfs(v);
            }
            if (on_stack[v]) {
                low_links[u] = std::min(low_links[u], low_links[v]);
            }
        }
        if (ids[u] == low_links[u]) {
            while (true) {
                int node = st.top();
                st.pop();
                on_stack[node] = false;
                low_links[node] = ids[u];
                scc_map[node] = scc_count;
                if (node == u) {
                    break;
                }
            }
            ++scc_count;
        }
    };

    for (int i = 0; i < n; ++i) {
        if (ids[i] == -1) {
            dfs(i);
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

    for (int i = 0; i < scc_count; ++i) {
        std::sort(adj_list[i].begin(), adj_list[i].end());
        adj_list[i].erase(std::unique(adj_list[i].begin(), adj_list[i].end()), adj_list[i].end());
    }
}

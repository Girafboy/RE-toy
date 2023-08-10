#include "Graph.h"

#include <utility>
#include <fstream>
#include <sstream>
#include <random>
#include <unordered_set>

Graph::Graph(std::vector<std::vector<int>> adj_list, std::string name) : adj_list(std::move(adj_list)), name(std::move(name)) {
    generateReverseAdjList();
    n_node = adj_list.size();
    n_edge = 0;
    for (auto list : adj_list) {
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
    std::ifstream graph_file;
    graph_file.open(input_file_path);
    std::string line;
    while (std::getline(graph_file, line)) {
        std::istringstream iss(line);
        int node = 0;
        iss >> node;
        adj_list.emplace_back();
        while (iss >> node) {
            adj_list.back().push_back(node);
        }
    }
    graph_file.close();
    generateReverseAdjList();

    n_node = adj_list.size();
    n_edge = 0;
    for (const auto& list : adj_list) {
        n_edge += list.size();
    }
}

Graph::Graph(std::istream &in, std::string name) : name(std::move(name)) {
    std::string buf;
    std::getline(in, buf);

    strTrimRight(buf);

    int n;
    getline(in, buf);
    std::istringstream(buf) >> n;

    std::string sub;
    int idx;
    int sid = 0;
    int tid = 0;
    while (getline(in, buf)) {
        adj_list.emplace_back();

        strTrimRight(buf);
        idx = buf.find(":");
        buf.erase(0, idx + 2);
        while (buf.find(" ") != std::string::npos) {
            sub = buf.substr(0, buf.find(" "));
            std::istringstream(sub) >> tid;
            buf.erase(0, buf.find(" ") + 1);

            adj_list.back().push_back(tid);
        }
        ++sid;
    }
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

std::vector<int> Graph::getPredecessors(int i) const {
    return reverse_adj_list[i];
}

std::vector<int> Graph::getSuccessors(int i) const {
    return adj_list[i];
}

std::string Graph::getName() const {
    return name;
}

void Graph::strTrimRight(std::string &str) {
    std::string whitespaces(" \t");
    size_t index = str.find_last_not_of(whitespaces);
    if (index != std::string::npos)
        str.erase(index + 1);
    else
        str.clear();
}

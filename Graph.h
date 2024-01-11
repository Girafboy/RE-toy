#ifndef TC_GRAPH_H
#define TC_GRAPH_H

#include <vector>
#include <string>

class Graph {
private:
    std::string name;
    std::size_t n_node;  // number of nodes after merging scc
    std::size_t n_edge;  // number of edges after merging scc
    std::vector<std::vector<int>> adj_list;  // adjacency list after merging scc
    std::vector<std::vector<int>> reverse_adj_list;  // inverse adjacency list after merging scc
    std::vector<int> scc_map;  // map from original node to new node (scc id)

    void generateReverseAdjList();

    void mergeScc(const std::vector<std::vector<int>> &original_adj_list);

public:
    explicit Graph(const std::vector<std::vector<int>>& original_adj_list, std::string name = "graph");

    explicit Graph(const std::string &input_file_path, std::string name = "graph");

    explicit Graph(int n, int d, int seed, std::string name = "graph");

    size_t getInDegree(int i) const;

    size_t getOutDegree(int i) const;

    size_t size() const;

    size_t number_of_edges() const;

    size_t number_of_original_nodes() const;

    std::vector<int> getPredecessors(int i) const;

    std::vector<int> getSuccessors(int i) const;

    std::string getName() const;

    int getSccId(int i) const;
};


#endif

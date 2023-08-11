#ifndef TC_GRAPH_H
#define TC_GRAPH_H

#include <vector>
#include <string>

class Graph {
private:
    std::string name;
    std::size_t n_node;
    std::size_t n_edge;
    std::vector<std::vector<int>> adj_list;
    std::vector<std::vector<int>> reverse_adj_list;

    void generateReverseAdjList();

    static void strTrimRight(std::string &str);

public:
    explicit Graph(std::vector<std::vector<int>> adj_list, std::string name = "graph");

    explicit Graph(const std::string &input_file_path, std::string name = "graph");

    explicit Graph(std::istream &in, std::string name);  // read .gra file

    explicit Graph(int n, int d, std::string name = "graph");

    size_t getInDegree(int i) const;

    size_t getOutDegree(int i) const;

    size_t size() const;

    size_t number_of_edges() const;

    std::vector<int> getPredecessors(int i) const;

    std::vector<int> getSuccessors(int i) const;

    std::string getName() const;
};


#endif

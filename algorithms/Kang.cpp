//
// Created by 王星力 on 2022/10/25.
//

#include "Kang.h"

#include <cmath>
#include <cstddef>
#include <fstream>
#include <iterator>
#include <sstream>
#include <iostream>
#include <queue>
#include <stack>
#include <functional>
#include <utility>
#include <unordered_set>
#include <climits>
#include <algorithm>
#include <cstring>
#include <vector>
#include <math.h>

namespace kang {

    std::vector<Kang::Node> Kang::nodes;
    int Kang::visit_cur = 0;
    int Kang::color_cur = 0;


    void Kang::volume_update(std::vector<int>& main, std::vector<int>& minor) {
        color_cur++;
        for (auto v: main) {
            nodes[v].color = color_cur;
        }
        for (auto v: minor) {
            nodes[v].color = color_cur + 1;
        }

        std::queue<int> q;

        // from up to down
        for (auto i: main) {
            nodes[i].volume_in = 0;
            for (auto v: graph->getPredecessors(i)) {
                if (nodes[v].color >= color_cur){
                    nodes[i].degree_in++;
                }
            }
        }
        for (auto i: minor) {
            nodes[i].volume_in = 0;
            for (auto v: graph->getPredecessors(i)) {
                if (nodes[v].color >= color_cur){
                    nodes[i].degree_in++;
                }
            }
        }
        for (auto i: main) {
            if (nodes[i].degree_in == 0) {
                q.push(i);
            }
        }
        for (auto i: minor) {
            if (nodes[i].degree_in == 0) {
                q.push(i);
            }
        }
        while (!q.empty()) {
            int cur = q.front(); q.pop();
            int deg = 0;
            for (auto v: graph->getSuccessors(cur)) {
                if (nodes[v].color >= color_cur){
                    deg++;
                }
            }
            for (auto v: graph->getSuccessors(cur)) {
                if (nodes[v].color >= color_cur){
                    if (!(--nodes[v].degree_in)) {
                        q.push(v);
                    }
                    nodes[v].volume_in += (nodes[cur].volume_in/deg + 1);
                }
            }
        }
        // from down to up
        for (auto i: main) {
            nodes[i].volume_out = 0;
            for (auto v: graph->getSuccessors(i)) {
                if (nodes[v].color >= color_cur){
                    nodes[i].degree_out++;                
                }
            }
        }
        for (auto i: minor) {
            nodes[i].volume_out = 0;
            for (auto v: graph->getSuccessors(i)) {
                if (nodes[v].color >= color_cur){
                    nodes[i].degree_out++;                
                }
            }
        }
        for (auto i: main) {
            if (nodes[i].degree_out == 0) {
                q.push(i);
            }
        }
        for (auto i: minor) {
            if (nodes[i].degree_out == 0) {
                q.push(i);
            }
        }
        while (!q.empty()) {
            int cur = q.front(); q.pop();
            int deg = 0;
            for (auto v: graph->getPredecessors(cur)) {
                if (nodes[v].color >= color_cur){
                    deg++;
                }
            }
            for (auto v: graph->getPredecessors(cur)) {
                if (nodes[v].color >= color_cur){
                    if (!(--nodes[v].degree_out)) {
                        q.push(v);
                    }
                    nodes[v].volume_out += (nodes[cur].volume_out/deg + (nodes[v].color == color_cur));
                }
            }
        }
        color_cur++;

        cutoff(main, minor);
    }


    void Kang::cutoff(std::vector<int>& main, std::vector<int>& minor) {
        if (main.empty()) {
            return;
        }

        color_cur++;
        int split_node = main[0];
        for (auto v: main) {
            nodes[v].color = color_cur;
            if (nodes[v].volume_in*nodes[v].volume_out > nodes[split_node].volume_in*nodes[split_node].volume_out) {
                split_node = v;
            }
        }
        
        for (auto v: minor) {
            nodes[v].color = color_cur;
        }
        nodes[split_node].color = color_cur+3; nodes[split_node].volume_in = 0; nodes[split_node].volume_out = 0;

        std::vector<int> A, B, C;
        std::stack<int> stack;

        // from split_node to up
        stack.push(split_node);
        while (!stack.empty()) {
            int cur = stack.top(); stack.pop();
            for (auto v: graph->getPredecessors(cur)) {
                if (nodes[v].color == color_cur) {
                    nodes[v].color = color_cur + 1;
                    nodes[v].jump_out.push_back(split_node);
                    stack.push(v);
                }
            }
        }

        // from split_node to down
        stack.push(split_node);
        while (!stack.empty()) {
            int cur = stack.top(); stack.pop();
            for (auto v: graph->getSuccessors(cur)) {
                if (nodes[v].color == color_cur) {
                    nodes[v].color = color_cur + 2;
                    nodes[v].jump_in.push_back(split_node);
                    stack.push(v);
                }
            }
        }


        if (minor.empty()) {
            for (auto v: main) {
                if (nodes[v].color == color_cur+1) {
                    A.push_back(v);
                } else if (nodes[v].color == color_cur+2) {
                    B.push_back(v);
                } else if (nodes[v].color == color_cur) {
                    C.push_back(v);
                }
            }

            color_cur+=3;

            volume_update(B, C);
            std::copy(C.begin(), C.end(), std::back_inserter(A)); C.clear();
            volume_update(A, C);
        } else {
            for (auto v: main) {
                if (nodes[v].color == color_cur || nodes[v].color == color_cur+1) {
                    A.push_back(v);
                } else if (nodes[v].color == color_cur+2) {
                    B.push_back(v);
                }
                if (nodes[v].color == color_cur) {
                    C.push_back(v);
                }
            }
            for (auto v: minor) {
                if (nodes[v].color == color_cur) {
                    C.push_back(v);
                }
            }
            color_cur+=3;

            volume_update(A, minor);
            volume_update(B, C);
        }
    }


    Kang::Kang(int x) : x(x) {}

    void Kang::reset() {
        nodes.clear();
        topo_adj_list.clear();
        visit_cur = 0;
        color_cur = 0;
    }

    void Kang::construction(const Graph &graph) {
        this->graph = &graph;
        nodes.resize(graph.size());

        std::vector<int> main, minor;
        for (int i = 0; i < graph.size(); i++) {
            main.push_back(i);
        }
        volume_update(main, minor);
        for (int i = 0; i < graph.size(); i++) {
            std::sort(nodes[i].jump_in.begin(), nodes[i].jump_in.end());
            std::sort(nodes[i].jump_out.begin(), nodes[i].jump_out.end());
        }

        // std::vector<int> storage; storage.resize(graph.size());
        // std::vector<int> usage; usage.resize(graph.size());
        // for (int i = 0; i < graph.size(); i++) {
        //     for (int j = 0; j < graph.size(); j++) {
        //         bool flag = false;
        //         if (i == j) {
        //             std::cout << '+';
        //         } else {
        //             for (auto v: nodes[j].jump_in) {
        //                 if (v == i) {
        //                     std::cout << "O";
        //                     storage[j] += 1;
        //                     flag = true;
        //                 }
        //             }
        //             for (auto v: nodes[i].jump_out) {
        //                 if (v == j) {
        //                     std::cout << "I";
        //                     usage[j] += 1;
        //                     flag = true;
        //                 }
        //             }
        //             if (!flag) {
        //                 // if (TC_haspath(i, j)) {
        //                 //     std::cout << "_";
        //                 // } else {
        //                     std::cout << " ";
        //                 // }
        //             }
        //         }
        //     }
        //     // std::cout << "\t" << storage[i] << "\t" << usage[i]  << "\t" << (storage[i]+usage[i])*100/(storage[i]*(usage[i]+1)) << "%" << std::endl;
        //     std::cout << std::endl;
        // }
    }

    bool Kang::TC_haspath(int source, int target) {
        if (source == target) return true;
        if (!nodes[source].jump_out.empty()) {
            for (auto v: nodes[source].jump_out) {
                if (v == target) return true;
            }
        }
        if (!nodes[target].jump_in.empty()) {
            for (auto v: nodes[target].jump_in) {
                if (v == source) return true;
            }
        }
        if (nodes[source].jump_out.empty() || nodes[target].jump_in.empty()) {
            return false;
        }

        int ps = 0, pt = 0;
        int m = nodes[source].jump_out.size(), n = nodes[target].jump_in.size();
        int cur;
        while (ps < m && pt < n) {
            if (nodes[source].jump_out[ps] < nodes[target].jump_in[pt]) {
                ps++;
            } else if (nodes[source].jump_out[ps] > nodes[target].jump_in[pt]) {
                pt++;
            } else {
                return true;
            }
        }
        return false;
    }

    std::string Kang::getName() const {
        return "x-Jump";
    }

    std::string Kang::getParams() const {
        return "x=" + std::to_string(x);
    }

    long long Kang::getIndexSize() const {
        long long index_size = nodes.size() * (3 * sizeof(int));
        for (const auto &node: nodes) {
            index_size += node.jump_in.size() * sizeof(int);
            index_size += node.jump_out.size() * sizeof(int);
        }
        return index_size;
    }

}

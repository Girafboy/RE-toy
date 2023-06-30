//
// Created by 王星力 on 2022/10/25.
//

#include "Kang.h"

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

namespace kang {

    std::vector<Kang::Node> Kang::nodes;

    Kang::Jumps* Kang::merge1(Jumps *jumps) {
        static auto cmp = [](const int left, const int right) {
            return nodes[left].preorder < nodes[right].preorder;
        };

        Jumps *result = new Jumps ();
        std::sort(std::begin(*jumps), std::end(*jumps), cmp);

        int cur, last_postorder = INT_MIN;
        for (auto j: *jumps) {
            if (nodes[j].postorder > last_postorder) {
                result->push_back(j);
                last_postorder = nodes[j].postorder;
            }
        }
        return result;
    }

    Kang::Jumps* Kang::merge2(const Jumps *jumps_a, const Jumps *jumps_b) {
        Jumps * result = new Jumps ();
        result->reserve(jumps_a->size() + jumps_b->size());
        auto aptr = jumps_a->begin(), bptr = jumps_b->begin();

        int cur, last_postorder = INT_MIN;
        while (aptr != jumps_a->end()) {
            if (bptr == jumps_b->end()) {
                while (aptr != jumps_a->end()) {
                    if (nodes[*aptr].postorder > last_postorder) break;
                    aptr++;
                }
                result->insert(result->end(), aptr, jumps_a->end());
                result->shrink_to_fit();
                break;
            }
            if (nodes[*aptr].preorder < nodes[*bptr].preorder) {
                cur = *aptr;
                aptr++;
            } else {
                cur = *bptr;
                bptr++;
            }
            if (nodes[cur].postorder > last_postorder) {
                result->push_back(cur);
                last_postorder = nodes[cur].postorder;
            }
        }
        while (bptr != jumps_b->end()) {
            if (nodes[*bptr].postorder > last_postorder) break;
            bptr++;
        }
        result->insert(result->end(), bptr, jumps_b->end());
        result->shrink_to_fit();
        return result;
    }

    Kang::Jumps* Kang::mergek(const std::vector<const Jumps *> &jumps_list) {
        static auto cmp = [](const Jumps *left, const Jumps *right) {
            return left->size() > right->size();
        };
        std::priority_queue<const Jumps *, std::vector<const Jumps *>, decltype(cmp)> heap(jumps_list.begin(), jumps_list.end(), cmp);

        std::vector<Jumps *> tmp;
        while (heap.size() > 1) {
            const Jumps *resl = heap.top();
            heap.pop();
            const Jumps *resr = heap.top();
            heap.pop();
            tmp.push_back(merge2(resl, resr));
            heap.push(tmp.back());
        }

        Jumps *result = new Jumps (*heap.top());
        for (auto t: tmp)
            delete t;

        return result;
    }


    void Kang::DFS(int u) {
        static int increno = 0;
        nodes[u].preorder = ++increno;
        bool flag;

        for (auto it = topo_adj_list[u].begin(); it != topo_adj_list[u].end(); ++it) {
            if (nodes[*it].preorder == 0) {
                DFS(*it);
            } else if (nodes[*it].preorder >= nodes[u].preorder) {
                topo_adj_list[u].erase(it--);
            }
        }
        nodes[u].postorder = increno;

        for (const int v: topo_adj_list[u]) {
            nodes[u].levels_count[nodes[v].level]++;            
            if (!nodes[v].level) {
                for (int i=0; i<MAX_LEVEL; i++) {
                    nodes[u].levels_count[i] += nodes[v].levels_count[i];
                }
            }
        }
        for (int i=MAX_LEVEL-1; i>=0; i--) {
            if (nodes[u].levels_count[i] >= x) {
                nodes[u].level = i + 1;
                break;
            }
        }
        // Tranvers child nodes and union all shallowjumps.
        Jumps *jump_tmp = new Jumps();
        if (nodes[u].level) {
            std::vector<const Jumps *> tmp_shallow;

            static int visit_cur = 0;
            visit_cur++;
            std::stack<int> stack;
            stack.push(u);
            while (!stack.empty()) {
                int cur = stack.top();
                stack.pop();

                for (auto v: topo_adj_list[cur]) {
                    if (nodes[v].visit != visit_cur) {
                        nodes[v].visit = visit_cur;
                        jump_tmp->push_back(v);
                        if (nodes[v].level) {
                            tmp_shallow.push_back(nodes[v].jump);
                        } else {
                            stack.push(v);
                        }
                    }
                }
            }
            auto tmp = nodes[u].jump;
            nodes[u].jump = merge1(jump_tmp);
            delete tmp;

            tmp_shallow.push_back(nodes[u].jump);
            tmp = nodes[u].jump;
            nodes[u].jump = new Jumps ();
            nodes[u].jump = mergek(tmp_shallow);
            delete tmp;
        } else {
            for (const int v: topo_adj_list[u]) {
                jump_tmp->push_back(v);
            }
            auto tmp = nodes[u].jump;
            nodes[u].jump = merge1(jump_tmp);
            delete tmp;
        }
    }


    bool Kang::cover(const Jumps &sortedlist, const int target_order) {
        // for (int u: sortedlist) {
        //     std::cout << "(" << nodes[u].preorder << "," << nodes[u].postorder << ")";
        // }
        // std::cout << std::endl;
        auto len = sortedlist.size();
        const int *first = &sortedlist[0], *mid;
        while (len) {
            mid = first + (len >> 1);
            if (target_order < nodes[*mid].preorder)
                len = (len >> 1);
            else {
                first = mid + 1;
                len -= (len >> 1) + 1;
            }
        }
        if (first != &sortedlist[0] && target_order <= nodes[*(first - 1)].postorder) {
            return true;
        }
        return false;
    }

    bool Kang::TC_haspath(const int source, const int target) {
        Node *source_ = &nodes[source], *target_ = &nodes[target];
        if (target_->preorder > source_->postorder) {
            return false;
        } else if (target_->preorder >= source_->preorder) {
            return true;
        } else if (source_->preorder <= target_->postorder) {
            return false;
        }

        if (cover(*(source_->jump), target_->preorder)) {
            return true;
        }
        if (source_->level) {
            return false;
        }

        static int visit_cur = 0;
        ++visit_cur;

        std::stack<Node *> openlist;
        openlist.push(source_);
        while (!openlist.empty()) {
            source_ = openlist.top();
            openlist.pop();

            for (auto v: *source_->jump) {
                if (nodes[v].visit != visit_cur) {
                    if (cover(*(nodes[v].jump), target_->preorder)) {
                        return true;
                    }
                    if (!nodes[v].level) {
                        openlist.push(&nodes[v]);                        
                    }
                    nodes[v].visit = visit_cur;
                }
            }
        }
        return false;
    }

    Kang::Kang(int x) : x(x) {}

    void Kang::reset() {
        for (auto node: nodes) {
            if (!node.jump) delete node.jump;
        }
        nodes.clear();
        topo_adj_list.clear();
    }

    void Kang::construction(const Graph &graph) {
        size_t n = graph.size();
        size_t m = graph.number_of_edges();
        while (m) {
            MAX_LEVEL++;
            m /= x;
        }

        nodes.resize(n);
        topo_adj_list.resize(n);
        for (int i = 0; i < n; i++) {
            nodes[i].levels_count = new unsigned char[MAX_LEVEL]; // TODO: log_x (m)
            memset(nodes[i].levels_count, 0, MAX_LEVEL*sizeof(unsigned char));
        }

        // in ascending topo-order
        std::queue<int> q;
        std::vector<size_t> in_degree(n);
        for (int i = 0; i < n; ++i) {
            in_degree[i] = graph.getInDegree(i);
        }
        for (int i = 0; i < n; ++i) {
            if (in_degree[i] == 0) {
                q.push(i);
            }
        }
        while (!q.empty()) {
            int current = q.front();
            q.pop();
            // construct topo_adj_list
            for (const int head: graph.getPredecessors(current)) {
                topo_adj_list[head].push_back(current);
            }
            for (const int tail: graph.getSuccessors(current)) {
                if (--in_degree[tail] == 0) {
                    q.push(tail);
                }
            }
        }

        for (int i = 0; i < n; ++i) {
            if (graph.getInDegree(i) == 0) {
                DFS(i);
            }
        }

        for (int i = 0; i < n; ++i) {
            nodes[i].visit = 0;
        }
    }

    std::string Kang::getName() const {
        return "x-Jump";
    }

    std::string Kang::getParams() const {
        return "x=" + std::to_string(x);
    }

    long long Kang::getIndexSize() const {
        long long index_size = nodes.size() * (3 * sizeof(int));
        index_size += nodes.size() * MAX_LEVEL * sizeof(unsigned char);
        for (const auto &node: nodes) {
            index_size += ((node.jump ? node.jump->size() : 0)) * sizeof(int);
        }
        return index_size;
    }

}

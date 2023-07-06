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
    int Kang::increno = 0;

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

        // Traverse child nodes and union all shallowjumps.
        visit_cur++;
        Jumps tmp_jump {u}, tmp_deep(topo_adj_list[u].begin(), topo_adj_list[u].end());
        nodes[u].deep = merge1(&tmp_deep);
        tmp_deep = *nodes[u].deep;
        for (auto v: *nodes[u].deep) {
            nodes[v].visit = visit_cur;
            if (nodes[v].deep) {
                for (auto d: *nodes[v].deep) {
                    if (nodes[d].visit != visit_cur) {
                        tmp_deep.push_back(d);
                        nodes[d].visit = visit_cur;
                    }
                }
            }
        }
        delete nodes[u].deep;
        // int x_new = 1, tmp = nodes.size() - visit_cur + 1;
        // while (tmp < nodes.size()) {
        //     tmp *= x;
        //     x_new *= x;
        // }
        // int x_new = x * std::log2(nodes.size()/(nodes.size() - nodes[u].preorder + 1));
        if (tmp_deep.size() > x) {
            static auto cmp = [](const int left, const int right) {
                return nodes[left].jump->size() < nodes[right].jump->size();
            };
            std::nth_element(tmp_deep.begin(), tmp_deep.end()-x, tmp_deep.end(), cmp);
            nodes[u].deep = new Jumps(tmp_deep.end()-x, tmp_deep.end());

            std::vector<const Jumps *> tmp_shallow;

            for (auto it = tmp_deep.begin(); it != tmp_deep.end()-x; it++) {
                if (nodes[*it].jump) {
                    tmp_shallow.push_back(nodes[*it].jump);
                }
            }
            tmp_shallow.push_back(&tmp_jump);
            nodes[u].jump = mergek(tmp_shallow);

            tmp_jump.clear();
            for (auto j: *nodes[u].jump) {
                bool flag = true;
                for (auto v: *nodes[u].deep) {
                    if (cover(*(nodes[v].jump), nodes[j].preorder)) {
                        flag = false;
                        break;
                    }
                }
                if (flag) {
                    tmp_jump.push_back(j);
                }
            }
            delete nodes[u].jump;
            nodes[u].jump = new Jumps(tmp_jump);
        } else {
            nodes[u].deep = new Jumps(tmp_deep.begin(), tmp_deep.end());
            nodes[u].jump = new Jumps(tmp_jump);
        }
        nodes[u].level = std::log2(nodes[u].jump->size());
    }


    bool Kang::cover(const Jumps &sortedlist, const int target_order) {
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
        if (nodes[target].preorder > nodes[source].postorder) {
            return false;
        } else if (nodes[target].preorder >= nodes[source].preorder) {
            return true;
        } else if (nodes[source].preorder <= nodes[target].postorder) {
            return false;
        }

        if (cover(*nodes[source].jump, nodes[target].preorder)) {
            return true;
        }

        for (auto v: *nodes[source].deep) {
            if (cover(*nodes[v].jump, nodes[target].preorder)) {
                return true;
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
        visit_cur = 0;
        increno = 0;
    }

    void Kang::construction(const Graph &graph) {
        size_t n = graph.size();
        MAX_LEVEL = 30;

        nodes.resize(n);
        topo_adj_list.resize(n);

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

        // std::vector<int> level_count, jump_size, deep_size;
        // level_count.resize(MAX_LEVEL);
        // jump_size.resize(MAX_LEVEL);
        // deep_size.resize(MAX_LEVEL);
        // for (int i = 0; i < n; ++i) {
        //     level_count[nodes[i].level] ++;
        //     jump_size[nodes[i].level] += nodes[i].jump->size();
        // }
        // for (int i = 0; i < level_count.size(); ++i) {
        //     std::cout << "level-" << i << ": \t" << level_count[i] << "\t" << jump_size[i] << "\t" << jump_size[i]/(level_count[i]+1) << std::endl;
        // }
        std::vector<int> storage; storage.resize(n);
        std::vector<int> usage; usage.resize(n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                bool flag = false;
                for (auto v: *nodes[i].jump) {
                    if (v == j) {
                        std::cout << "X";
                        storage[i] += 1;
                        flag = true;
                    }
                }
                for (auto v: *nodes[i].deep) {
                    if (v == j) {
                        std::cout << "O";
                        usage[j] += 1;
                        flag = true;
                    }
                }
                if (!flag) {
                    if (TC_haspath(i, j)) {
                        std::cout << "_";
                    } else {
                        std::cout << " ";
                    }
                }
            }
            std::cout << "\t" << storage[i] << "\t" << usage[i]  << "\t" << (storage[i]+usage[i])*100/(storage[i]*(usage[i]+1)) << "%" << std::endl;
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
        for (const auto &node: nodes) {
            index_size += ((node.jump ? node.jump->size() : 0)) * sizeof(int);
            index_size += ((node.deep ? node.deep->size() : 0)) * sizeof(int);
        }
        return index_size;
    }

}

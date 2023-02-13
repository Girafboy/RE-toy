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

namespace kang {

    std::vector<Kang::Node> Kang::nodes;

    void Kang::merge2(std::vector<int> *a, std::vector<int> *b, std::vector<int> &result) {
        result.reserve(a->size() + b->size());
        auto aptr = a->begin(), bptr = b->begin();

        int cur, last_postorder = INT_MIN;
        while (aptr != a->end()) {
            if (bptr == b->end()) {
                while (aptr != a->end()) {
                    if (nodes[*aptr].postorder > last_postorder) break;
                    aptr++;
                }
                result.insert(result.end(), aptr, a->end());
                result.shrink_to_fit();
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
                result.push_back(cur);
                last_postorder = nodes[cur].postorder;
            }
        }
        while (bptr != b->end()) {
            if (nodes[*bptr].postorder > last_postorder) break;
            bptr++;
        }
        result.insert(result.end(), bptr, b->end());
        result.shrink_to_fit();
    }

    void Kang::merge(std::vector<std::vector<int> *> &jumps, std::vector<int> &merged_jump) {
        if (jumps.empty()) return;

        static auto cmp = [](const std::vector<int> *left, const std::vector<int> *right) {
            return left->size() > right->size();
        };
        std::priority_queue<std::vector<int> *, std::vector<std::vector<int> *>, decltype(cmp)> heap(jumps.begin(),
                                                                                                     jumps.end(), cmp);

        std::vector<std::vector<int> *> tmp;
        while (heap.size() > 1) {
            tmp.push_back(new std::vector<int>());
            std::vector<int> *resl = heap.top();
            heap.pop();
            std::vector<int> *resr = heap.top();
            heap.pop();
            merge2(resl, resr, *tmp.back());
            heap.push(tmp.back());
        }

        merged_jump.resize(heap.top()->size());
        std::copy(heap.top()->begin(), heap.top()->end(), merged_jump.begin());

        for (auto t: tmp)
            delete t;
    }

// O(kdf + f log f)
    void Kang::shrink(std::vector<int> &fakejump) {
        std::vector<std::pair<int, int>> indices;
        for (int i = 0; i < fakejump.size() - 1; ++i) {
            if (nodes[fakejump[i + 1]].preorder - nodes[fakejump[i]].postorder > 1) {
                indices.emplace_back(nodes[fakejump[i + 1]].preorder - nodes[fakejump[i]].postorder, i);
            }
        }

        if (indices.size() > x) {
            std::nth_element(indices.begin(), indices.begin() + x, indices.end(), std::greater<void>());
        }
        std::vector<int> slice;
        for (int i = 0; i < std::min(indices.size(), x); ++i) {
            slice.push_back(indices[i].second);
        }
        slice.push_back(-1);
        slice.push_back(fakejump.size() - 1);
        std::sort(slice.begin(), slice.end());

        std::vector<int> old_fakejump(fakejump);
        fakejump.clear();
        for (auto it = slice.begin() + 1; it != slice.end(); ++it)
            if (*prev(it) + 1 != *it) {
                nodes.emplace_back(nodes[old_fakejump[*prev(it) + 1]].preorder, nodes[old_fakejump[*it]].postorder);
                fakejump.push_back(nodes.size() - 1);
            } else {
                fakejump.push_back(old_fakejump[*it]);
            }
    }

    void Kang::DFS(int u) {
        static int increno = 0;
        nodes[u].preorder = ++increno;
        bool flag;
        std::set<int>::iterator pos;
        for (auto it = topo_adj_list[u].begin(); it != topo_adj_list[u].end(); ++it) {
            if (nodes[*it].preorder == 0) {
                DFS(*it);
            } else if (nodes[*it].preorder >= nodes[u].preorder) {
                topo_adj_list[u].erase(it--);
            }
        }
        nodes[u].postorder = increno;

        std::vector<std::vector<int> *> tmp_shallow, tmp_deep, tmp;

        nodes[u].shallowjump = new std::vector<int>();
        nodes[u].deepjump = new std::vector<int>();
        tmp_deep.push_back(new std::vector<int>());
        tmp_shallow.push_back(new std::vector<int>());
        for (const int v: topo_adj_list[u])
            if (nodes[v].shallowjump->size() + nodes[v].deepjump->size() > x) {
                tmp.push_back(new std::vector<int>{v});
                tmp_deep.push_back(tmp.back());
            } else {
                tmp.push_back(new std::vector<int>{v});
                tmp_shallow.push_back(tmp.back());
                if (nodes[v].shallowjump) {
                    tmp_shallow.push_back(nodes[v].shallowjump);
                }
                if (nodes[v].deepjump) {
                    tmp_deep.push_back(nodes[v].deepjump);
                }
            }
        merge(tmp_deep, *nodes[u].deepjump);
        merge(tmp_shallow, *nodes[u].shallowjump);
        // merge(tmp_shallow, 0, tmp_shallow.size()-1, *nodes[u].shallowjump);
        // merge(tmp_deep, 0, tmp_deep.size()-1, *nodes[u].deepjump);
        for (auto t: tmp) delete t;

        if (nodes[u].shallowjump->size() + nodes[u].deepjump->size() > x) {
            nodes[u].fakejump = new std::vector<int>();
            std::vector<std::vector<int> *> tmp_fake = {new std::vector<int>{u}};
            if (nodes[u].shallowjump) {
                tmp_fake.push_back(nodes[u].shallowjump);
            }
            for (const int v: *(nodes[u].deepjump)) {
                if (nodes[v].fakejump) {
                    tmp_fake.push_back(nodes[v].fakejump);
                }
            }
            merge(tmp_fake, *(nodes[u].fakejump));
            // merge(tmp_fake, 0, tmp_fake.size()-1, *nodes[u].fakejump);
            shrink(*nodes[u].fakejump);

            delete tmp_fake[0];
        }
    }


    bool Kang::cover(const std::vector<int> &sortedlist, const int target_order) {
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
        }

        if (source_->fakejump && !cover(*(source_->fakejump), target_->preorder)) {
            return false;
        }

        static int visit_cur = 0;
        ++visit_cur;

        std::stack<Node *> openlist;
        openlist.push(source_);
        size_t len;
        const int *cur;
        while (!openlist.empty()) {
            source_ = openlist.top();
            openlist.pop();
            if (cover(*(source_->shallowjump), target_->preorder)) {
                return true;
            }

            for (len = source_->deepjump->size(), cur = &(source_->deepjump->back()); len; len--) {
                source_ = &nodes[*(cur--)];
                if (target_->preorder >= source_->preorder) {
                    if (target_->preorder <= source_->postorder) {
                        return true;
                    }
                    break;
                }
                if (source_->visit != visit_cur) {
                    source_->visit = visit_cur;
                    if (cover(*(source_->fakejump), target_->preorder)) {
                        openlist.push(source_);
                        // static int count = 0;
                        // std::cout << ++count << std::endl;
                    }
                }
            }
        }
        return false;
    }

    Kang::Kang(int x) : x(x) {}

    void Kang::reset() {
        for (auto node: nodes) {
            if (!node.shallowjump) delete node.shallowjump;
            if (!node.deepjump) delete node.deepjump;
            if (!node.fakejump) delete node.fakejump;
        }
        nodes.clear();
        topo_adj_list.clear();
    }

    void Kang::construction(const Graph &graph) {
        size_t n = graph.size();
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
    }

    std::string Kang::getName() const {
        return "x-Jump";
    }

    std::string Kang::getParams() const {
        return "x=" + std::to_string(x);
    }

    long long Kang::getIndexSize() const {
        long long index_size = nodes.size() * (2 * sizeof(int));
        for (const auto &node: nodes) {
            index_size +=
                    ((node.shallowjump ? node.shallowjump->size() : 0) + (node.deepjump ? node.deepjump->size() : 0) +
                     (node.fakejump ? node.fakejump->size() : 0)) * sizeof(int);
        }
        return index_size;
    }

}

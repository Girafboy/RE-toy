//
// Created by 王星力 on 2022/10/25.
//

#include "Kang.h"

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iomanip>
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

    Kang::Kang(int x) : x(x) {}

    void Kang::encode(const Bits &bits, Bits &out) {
        uint64_t lo = 0, hi = RANGE_MAX;
        double p0 = 1 - connect_probability;
        int pending = 0;
        for (int i = 0; i < bits.size; i++) {
            uint64_t range = std::min(RANGE_MAX, std::max(1UL, (uint64_t)((hi - lo + 1) * p0)));

            if (bits.get(i)) {
                lo = lo + range;
                p0 *= (1-connect_probability);
            } else {
                hi = lo + range - 1;
            }
            while (true) {
                if (hi < RANGE_HALF) {
                    out.append_zero();
                    while (pending) {
                        out.append_one();
                        pending--;
                    }
                } else if (lo >= RANGE_HALF) {
                    out.append_one();
                    while (pending) {
                        out.append_zero();
                        pending--;
                    }
                } else if (lo >= RANGE_ONE_QUAR && hi < RANGE_THREE_QUAR) {
                    pending++;
                    lo -= RANGE_ONE_QUAR;
                    hi -= RANGE_ONE_QUAR;
                } else {
                    break;
                }
                lo <<= 1;
                hi <<= 1; hi |= 1;
                lo &= RANGE_MAX;
                hi &= RANGE_MAX;
            }
        }
        pending++;
        if (lo < RANGE_ONE_QUAR) {
            out.append_zero();
            while (pending) {
                out.append_one();
                pending--;
            }
        } else {
            out.append_one();
            while (pending) {
                out.append_zero();
                pending--;
            }
        }
    }

    void Kang::decode(const Bits &code, Bits &out, int len) {
        uint64_t lo = 0, hi = RANGE_MAX, value = 0;
        int code_cur = 0;
        double p0 = 1 - connect_probability;

        for (; code_cur < 32; code_cur++) {
            value <<= 1;
            value |= (code_cur < code.size ? code.get(code_cur) : 0);
        }
        for (int i = 0; i < len; i++) {
            uint64_t range = std::min(RANGE_MAX, std::max(1UL, (uint64_t)((hi - lo + 1) * p0)));

            if (value >= lo + range) {
                lo = lo + range;
                out.set(out.size - len + i);
                p0 *= (1-connect_probability);
            } else {
                hi = lo + range - 1;
            }

            while (true) {
                if (hi < RANGE_HALF) {
                } else if (lo >= RANGE_HALF) {
                    lo -= RANGE_HALF;
                    hi -= RANGE_HALF;
                    value -= RANGE_HALF;
                } else if (lo >= RANGE_ONE_QUAR && hi < RANGE_THREE_QUAR) {
                    lo -= RANGE_ONE_QUAR;
                    hi -= RANGE_ONE_QUAR;
                    value -= RANGE_ONE_QUAR;
                } else {
                    break;
                }
                lo <<= 1;
                hi <<= 1; hi |= 1;
                value <<= 1; value |= (code_cur < code.size ? code.get(code_cur++) : 0);
            }
        }
    }

    void Kang::encode_decode_correctness_test() {
        connect_probability = 0.001;
        Bits bits, out, res;
        for (int i=0; i<100; i++) {
            bits.append_zero();
            bits.append_one();
        }
        res.init(bits.size);
        std::cout << "*********** ";
        bits.print();
        std::cout << " >> ";
        encode(bits, out);
        out.print();
        std::cout << " >> ";
        decode(out, res, bits.size);
        res.print();
        std::cout << " ***********";
    }

    void Kang::reset() {
        nodes.clear();
    }

    void Kang::construction(const Graph &graph) {
        // encode_decode_correctness_test();

        this->graph = &graph;
        size_t n = graph.size();
        nodes.resize(n);
        connect_probability = (double)graph.number_of_edges() / n / (n-1) * 2;
        
        std::queue<int> q;
        std::vector<size_t> out_degree(n);
        for (int i = 0; i < n; ++i) {
            out_degree[i] = graph.getOutDegree(i);
        }
        for (int i = 0; i < n; ++i) {
            if (out_degree[i] == 0) {
                q.push(i);
            }
        }
        int topo_order = 0;
        while (!q.empty()) {
            int cur = q.front(); q.pop();
            nodes[cur].topo_order = topo_order++;
            // ****************** encode start *****************//
            Bits res; res.init(nodes[cur].topo_order);
            for (const int v : graph.getSuccessors(cur)) {
                res.set(nodes[cur].topo_order - nodes[v].topo_order - 1);
                decode(nodes[v].code, res, nodes[v].topo_order);
            }
            encode(res, nodes[cur].code);
            // ****************** encode end *******************//

            for (const int u : graph.getPredecessors(cur)) {
                if (--out_degree[u] == 0) {
                    q.push(u);
                }
            }
        }
    }

    bool Kang::TC_haspath(int source, int target) {
        if (source == target) {
            return true;
        } else if (nodes[source].topo_order < nodes[target].topo_order) {
            return false;
        } else {
            Bits res; res.init(nodes[source].topo_order);
            decode(nodes[source].code, res, nodes[source].topo_order - nodes[target].topo_order);
            return res.get(nodes[source].topo_order - 1);
        }
    }

    std::string Kang::getName() const {
        return "x-Jump";
    }

    std::string Kang::getParams() const {
        return "";
    }

    long long Kang::getIndexSize() const {
        long long index_size = nodes.size() * sizeof(int);
        for (const auto &node: nodes) {
            index_size += node.code.size_bytes;
        }
        return index_size;
    }

}
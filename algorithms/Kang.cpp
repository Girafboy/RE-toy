#include "Kang.h"

#include <cassert>
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

// #define DEBUG

namespace kang {

    std::vector<Kang::Node> Kang::nodes;

    Kang::Kang(int x) : x(x) {}

    void Kang::encode(const Bits &bits, Bits &out, float &p0) {
        unsigned long long lo = 0, hi = RANGE_MAX;
        int pending = 0;
        for (int i = 0; i < bits.size; i++) {
            unsigned long long range = std::min((unsigned long long) RANGE_MAX, std::max(1ULL, (unsigned long long) ((hi - lo + 1) * p0)));

            if (bits.get(i)) {
                lo = lo + range;
                p0 *= (1-connect_probability[bits.size-i-1]);
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

    void Kang::decode(const Bits &code, Bits &out, float p0, int len) {
        unsigned long long lo = 0, hi = RANGE_MAX, value = 0;
        int code_cur = 0;

        for (; code_cur < 32; code_cur++) {
            value <<= 1;
            value |= (code_cur < code.size ? code.get(code_cur) : 0);
        }
        for (int i = 0; i < len; i++) {
            unsigned long long range = std::min((unsigned long long) RANGE_MAX, std::max(1ULL, (unsigned long long) ((hi - lo + 1) * p0)));

            if (value >= lo + range) {
                lo = lo + range;
                out.set(out.size - len + i);
                p0 *= (1-connect_probability[len-i-1]);
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

    // void Kang::encode_decode_correctness_test() {
    //     connect_probability = 0.001;
    //     Bits bits, out, res;
    //     for (int i=0; i<100; i++) {
    //         bits.append_zero();
    //         bits.append_one();
    //     }
    //     res.init(bits.size);
    //     std::cout << "*********** ";
    //     bits.print();
    //     std::cout << " >> ";
    //     float p0 = 1 - connect_probability;
    //     encode(bits, out, p0);
    //     out.print();
    //     std::cout << " >> ";
    //     decode(out, res, 1 - connect_probability, bits.size);
    //     res.print();
    //     std::cout << " ***********";
    // }

    void Kang::reset() {
        nodes.clear();
    }

    void Kang::construction(const Graph &graph) {
        // encode_decode_correctness_test();

        this->graph = &graph;
        size_t n = graph.size();
        nodes.resize(n);
        connect_probability = new float[n];
        std::memset(connect_probability, 0, sizeof(float)*n);
        
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
            if (nodes[cur].topo_order) {
                connect_probability[nodes[cur].topo_order] = (float)graph.getOutDegree(cur) / nodes[cur].topo_order;
                int chunks = (nodes[cur].topo_order+x-1)/x;
                nodes[cur].codes = new Bits[chunks];
                nodes[cur].p0_inits = new float[chunks];
                nodes[cur].nonzeros = new bool[chunks]; std::memset(nodes[cur].nonzeros, 0, sizeof(bool)*chunks);

                Bits res[chunks]; 
                for (int i = 0; i < chunks-1; i++) {
                    res[i].init(x);
                }
                res[chunks-1].init(nodes[cur].topo_order - x*(chunks-1));

                for (const int v : graph.getSuccessors(cur)) {
                    res[nodes[v].topo_order/x].set(res[nodes[v].topo_order/x].size - nodes[v].topo_order%x - 1);
                    nodes[cur].nonzeros[nodes[v].topo_order/x] = true;
                    if (nodes[v].topo_order) {
                        int chunks_v = (nodes[v].topo_order+x-1)/x;
                        for (int i = 0; i < chunks_v-1; i++) {
                            if (nodes[v].nonzeros[i]) {
                                decode(nodes[v].codes[i], res[i], nodes[v].p0_inits[i], x);
                                nodes[cur].nonzeros[i] = true;
                            }
                        }
                        if (nodes[v].nonzeros[chunks_v-1]) {
                            decode(nodes[v].codes[chunks_v-1], res[chunks_v-1], nodes[v].p0_inits[chunks_v-1], nodes[v].topo_order - x*(chunks_v-1));
                            nodes[cur].nonzeros[chunks_v-1] = true;
                        }
                    }
                }

                float p0 = 1 - connect_probability[nodes[cur].topo_order];
                nodes[cur].p0_inits[chunks-1] = p0;
                if (nodes[cur].nonzeros[chunks-1]) {
                    encode(res[chunks-1], nodes[cur].codes[chunks-1], p0);
                }
                for (int i = chunks-2; i >= 0 ; i--) {
                    nodes[cur].p0_inits[i] = p0;
                    if (nodes[cur].nonzeros[i]) {
                        encode(res[i], nodes[cur].codes[i], p0);
                    }
                }
            }
            // ****************** encode end *******************//
#ifdef DEBUG
            std::cout << nodes[cur].topo_order << " " << connect_probability[nodes[cur].topo_order] << ":";
            for (int i = 0; i < (nodes[cur].topo_order+x-1)/x; i++)
                std::cout << " " << nodes[cur].codes[i].size;
            std::cout << std::endl;
#endif
            for (const int u : graph.getPredecessors(cur)) {
                if (--out_degree[u] == 0) {
                    q.push(u);
                }
            }
        }
    }

    bool Kang::decode_check(const Bits &code, float p0, int len, int pos) {
        unsigned long long lo = 0, hi = RANGE_MAX, value = 0;
        int code_cur = 0;
        bool ret = false;

        for (; code_cur < 32; code_cur++) {
            value <<= 1;
            value |= (code_cur < code.size ? code.get(code_cur) : 0);
        }
        for (int i = 0; i < pos; i++) {
            unsigned long long range = std::min((unsigned long long) RANGE_MAX, std::max(1ULL, (unsigned long long) ((hi - lo + 1) * p0)));

            if (value >= lo + range) {
                lo = lo + range;
                ret = true;
                p0 *= (1-connect_probability[len-i-1]);
            } else {
                hi = lo + range - 1;
                ret = false;
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
        return ret;
    }

    bool Kang::TC_haspath(int source, int target) {
        if (source == target) {
            return true;
        } else if (nodes[source].topo_order < nodes[target].topo_order) {
            return false;
        } else {
            int chunks = (nodes[source].topo_order+x-1)/x, pos = nodes[target].topo_order/x;
            if (nodes[source].nonzeros[pos]) {
                if (pos == chunks-1) {
                    return decode_check(nodes[source].codes[pos], nodes[source].p0_inits[pos], nodes[source].topo_order - (chunks-1)*x, nodes[source].topo_order - (chunks-1)*x - nodes[target].topo_order%x);
                } else {
                    return decode_check(nodes[source].codes[pos], nodes[source].p0_inits[pos], x,  x - nodes[target].topo_order%x);
                }
            } else {
                return false;
            }
        }
    }

    std::string Kang::getName() const {
        return "x-Jump";
    }

    std::string Kang::getParams() const {
        return "x=" + std::to_string(x);
    }

    unsigned long long Kang::getIndexSize() const {
        long long index_size = nodes.size() * sizeof(int);
        for (const auto &node: nodes) {
            int chunks = (node.topo_order+x-1)/x;
            for (int i = 0; i < chunks; i++) {
                index_size += node.codes[i].size_bytes;
            }
            index_size += chunks * sizeof(float);
            index_size += chunks * sizeof(bool);
        }
        return index_size;
    }

}
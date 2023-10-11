#include "ReachabilityCoding.h"

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

namespace rc {

    ReachabilityCoding::ReachabilityCoding(int x) : chunk_size(x) {}

    int ReachabilityCoding::encode(const Bits &bits, Bits &out, float p0, int cur, int len) {
        unsigned long long lo = 0, hi = RANGE_MAX;
        int pending = 0;
        int ones = 0;
        for (int i = len-1; i >=0; i--) {
            unsigned long long range = std::min((unsigned long long) RANGE_MAX, std::max(1ULL, (unsigned long long) ((hi - lo + 1) * p0)));

            if (bits.get(i)) {
                lo = lo + range;
                p0 *= connect_p0[cur-len+i];
                ones++;
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
        return ones;
    }

    void ReachabilityCoding::reset() {
        nodes.clear();
    }

    void ReachabilityCoding::construction(const Graph &graph) {
        size_t n = graph.size();
        nodes.resize(n);
        connect_p0 = new float[n];
        
        std::queue<int> q;
        std::vector<size_t> out_degree(n), in_degree(n);
        for (int i = 0; i < n; ++i) {
            out_degree[i] = graph.getOutDegree(i);
            in_degree[i] = graph.getInDegree(i);
        }
        for (int i = 0; i < n; ++i) {
            if (out_degree[i] == 0) {
                q.push(i);
            }
        }
        int topo_order = 0;
        while (!q.empty()) {
            int cur = q.front(); q.pop();

            if (topo_order) {
                int chunks = get_chunk_num(topo_order);
                nodes[cur].codes = new Bits[chunks];
                connect_p0[topo_order] = 1.0 - (float)graph.getOutDegree(cur) / topo_order;

                for (int i = 0; i < chunks-1; i++) {
                    nodes[cur].codes[i].init(chunk_size);
                }
                nodes[cur].codes[chunks-1].init(topo_order - chunk_size*(chunks-1));

                for (const int v : graph.getSuccessors(cur)) {
                    nodes[cur].codes[nodes[v].topo_order/chunk_size].set(nodes[v].topo_order%chunk_size);
                    if (nodes[v].topo_order) {
                        int chunks_v = get_chunk_num(nodes[v].topo_order);
                        for (int i = 0; i < chunks_v; i++) {
                            nodes[cur].codes[i].bits_or(nodes[v].codes[i]);
                        }
                        if (--in_degree[v] == 0) {
                            // ****************** encode start *****************//
                            Bits *code_raw = nodes[v].codes;

                            nodes[v].codes = new Bits[chunks_v];
                            nodes[v].p0_pos = new int[get_p0_pos_num(nodes[v].topo_order)];
                            for (int j=0; j<get_p0_pos_num(nodes[v].topo_order); j++) {
                                nodes[v].p0_pos[j] = chunks_v-1;
                            }
                            int ones = 1 + encode(code_raw[chunks_v-1], nodes[v].codes[chunks_v-1], connect_p0[nodes[v].topo_order], nodes[v].topo_order, nodes[v].topo_order - chunk_size*(chunks_v-1));
                            for (int i = chunks_v-2; i >= 0 ; i--) {
                                for (int j=log2(ones); j<get_p0_pos_num(nodes[v].topo_order); j++) {
                                    nodes[v].p0_pos[j] = i;
                                }
                                ones += encode(code_raw[i], nodes[v].codes[i], get_p0(connect_p0[nodes[v].topo_order], ones), (i+1)*chunk_size, chunk_size);
                            }
                            delete [] code_raw;
                            // ****************** encode end *******************//
                        }
                    }
                }
                if (graph.getInDegree(cur) == 0) {
                    // ****************** encode start *****************//
                    Bits *code_raw = nodes[cur].codes;

                    nodes[cur].codes = new Bits[chunks];
                    nodes[cur].p0_pos = new int[get_p0_pos_num(topo_order)];
                    for (int j=0; j<get_p0_pos_num(topo_order); j++) {
                        nodes[cur].p0_pos[j] = chunks-1;
                    }
                    int ones = 1 + encode(code_raw[chunks-1], nodes[cur].codes[chunks-1], connect_p0[topo_order], topo_order, topo_order - chunk_size*(chunks-1));
                    for (int i = chunks-2; i >= 0 ; i--) {
                        for (int j=log2(ones); j<get_p0_pos_num(topo_order); j++) {
                            nodes[cur].p0_pos[j] = i;
                        }
                        ones += encode(code_raw[i], nodes[cur].codes[i], get_p0(connect_p0[topo_order], ones), (i+1)*chunk_size, chunk_size);
                    }
                    delete [] code_raw;
                    // ****************** encode end *******************//
                }

            }

#ifdef DEBUG
            std::cout << topo_order << " " << connect_p0[topo_order] << ":";
            for (int i = 0; i < (topo_order+x-1)/x; i++)
                std::cout << " " << get_p0(connect_p0[topo_order], i, nodes[cur].p0_pos) << " " << nodes[cur].codes[i].size;
            std::cout << std::endl;
            for (int i = 0; i < log2(topo_order)+1; i++)
                std::cout << " " << nodes[cur].p0_pos[i];
            std::cout << std::endl;
#endif
            nodes[cur].topo_order = topo_order++;
            for (const int u : graph.getPredecessors(cur)) {
                if (--out_degree[u] == 0) {
                    q.push(u);
                }
            }
        }
    }

    bool ReachabilityCoding::decode_check(const Bits &code, float p0, int cur, int len) {
        bool ret = false;
        unsigned long long lo = 0, hi = RANGE_MAX;
        unsigned long long value = ((code.data[0] << 24) | (code.data[1] << 16) | (code.data[2] << 8) | (code.data[3])) & (0xFFFFFFFF << (code.size<32 ? 32-code.size : 0));
        int code_cur = 32;

        for (int i = len-1; i >= 0; i--) {
            unsigned long long range = std::min((unsigned long long) RANGE_MAX, std::max(1ULL, (unsigned long long) ((hi - lo + 1) * p0)));

            if (value >= lo + range) {
                lo = lo + range;
                ret = true;
                p0 *= connect_p0[cur-len+i];
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

    bool ReachabilityCoding::TC_haspath(int source, int target) {
        if (source == target) {
            return true;
        } else if (nodes[source].topo_order < nodes[target].topo_order) {
            return false;
        } else {
            int chunks = get_chunk_num(nodes[source].topo_order), pos = nodes[target].topo_order/chunk_size;
            if (pos == chunks-1) {
                return decode_check(nodes[source].codes[pos], get_p0(connect_p0[nodes[source].topo_order], pos, nodes[source].p0_pos), nodes[source].topo_order, nodes[source].topo_order - nodes[target].topo_order);
            } else {
                return decode_check(nodes[source].codes[pos], get_p0(connect_p0[nodes[source].topo_order], pos, nodes[source].p0_pos), (pos+1)*chunk_size,  chunk_size - nodes[target].topo_order%chunk_size);
            }
        }
    }

    std::string ReachabilityCoding::getName() const {
        return "x-Jump";
    }

    std::string ReachabilityCoding::getParams() const {
        return "x=" + std::to_string(chunk_size);
    }

    unsigned long long ReachabilityCoding::getIndexSize() const {
        long long index_size = nodes.size() * sizeof(int);
        for (const auto &node: nodes) {
            int chunks = (node.topo_order+chunk_size-1)/chunk_size;
            for (int i = 0; i < chunks; i++) {
                index_size += node.codes[i].size_bytes;
            }
            index_size += get_p0_pos_num(node.topo_order) * sizeof(int);
        }
        return index_size;
    }

}
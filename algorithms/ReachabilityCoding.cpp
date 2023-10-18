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

    ReachabilityCoding::ReachabilityCoding(int x) : chunk_size(x-1) {}

    ReachabilityCoding::FastFloat ReachabilityCoding::encode(const Bits &bits, Bits &out, FastFloat p0, int cur, int len) {
        unsigned long long lo = 0, hi = RANGE_MAX;
        int pending = 0;
        for (int i = len-1; i >=0; i--) {
            unsigned long long range = std::min( RANGE_MAX, std::max(1U, p0 * (hi - lo)));

            if (bits.get(i)) {
                lo = lo + range;
                p0 = p0 * connect_p0[cur-len+i];
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

        if (out.size >= bits.size) {
            out = bits;
            out.append_one();
        } else {
            out.append_zero();
        }
        return p0;
    }

    void ReachabilityCoding::encode(ReachabilityCoding::Node &node) {
        int chunks = get_chunk_num(node.topo_order);

        Bits *code_raw = node.codes;

        node.codes = new Bits[chunks];
        FastFloat p0_base = connect_p0[node.topo_order];
        FastFloat p0 = encode(code_raw[chunks-1], node.codes[chunks-1], p0_base, node.topo_order, node.topo_order - chunk_size*(chunks-1));
        for (int i = chunks-2; i >= 0 ; i--) {
            if (approximation_ratio(p0, p0_base) > 2) {
                p0_base = p0;
                node.p0_pos.emplace_back(i, p0_base);
            }
            p0 = p0 * encode(code_raw[i], node.codes[i], p0_base, (i+1)*chunk_size, chunk_size) / p0_base;
        }
        delete [] code_raw;
    }

    void ReachabilityCoding::reset() {
        delete [] nodes;
        delete [] connect_p0;
    }

    void ReachabilityCoding::construction(const Graph &graph) {
        n = graph.size();
        nodes = new Node[n];
        connect_p0 = new FastFloat[n];
        
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
            nodes[cur].topo_order = topo_order++;

            if (nodes[cur].topo_order) {
                int chunks = get_chunk_num(nodes[cur].topo_order);
                nodes[cur].codes = new Bits[chunks];
                connect_p0[nodes[cur].topo_order] = FastFloat(nodes[cur].topo_order - graph.getOutDegree(cur), nodes[cur].topo_order);

                for (int i = 0; i < chunks-1; i++) {
                    nodes[cur].codes[i].init(chunk_size);
                }
                nodes[cur].codes[chunks-1].init(nodes[cur].topo_order - chunk_size*(chunks-1));

                for (const int v : graph.getSuccessors(cur)) {
                    nodes[cur].codes[nodes[v].topo_order/chunk_size].set(nodes[v].topo_order%chunk_size);
                    if (nodes[v].topo_order) {
                        int chunks_v = get_chunk_num(nodes[v].topo_order);
                        for (int i = 0; i < chunks_v; i++) {
                            nodes[cur].codes[i].bits_or(nodes[v].codes[i]);
                        }
                        if (--in_degree[v] == 0) {
                            encode(nodes[v]);
                        }
                    }
                }
                if (graph.getInDegree(cur) == 0) {
                    encode(nodes[cur]);
                }

            }

            for (const int u : graph.getPredecessors(cur)) {
                if (--out_degree[u] == 0) {
                    q.push(u);
                }
            }
        }
    }

    bool ReachabilityCoding::decode_check(const Bits &code, FastFloat p0, int cur, int len) {
        bool ret = false;
        unsigned long long lo = 0, hi = RANGE_MAX;
        unsigned long long value = ((code.data[0] << 24) | (code.data[1] << 16) | (code.data[2] << 8) | (code.data[3])) & (0xFFFFFFFF << (code.size<32 ? 32-code.size : 0));
        int code_cur = 32;

        for (int i = len-1; i >= 0; i--) {
            unsigned long long range = std::min(RANGE_MAX, std::max(1U, p0 * (hi - lo)));

            if (value >= lo + range) {
                lo = lo + range;
                ret = true;
                p0 = p0 * connect_p0[cur-len+i];
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
                value <<= 1; value |= (code_cur < code.size-1 ? code.get(code_cur++) : 0);
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
            Bits *bits = &nodes[source].codes[pos];
            int size = bits->size;
            if (bits->get(size-1)) {
                return bits->get(nodes[target].topo_order%chunk_size);
            }
            if (pos == chunks-1) {
                return decode_check(*bits, get_p0(nodes[source].p0_pos, pos, connect_p0[nodes[source].topo_order]), nodes[source].topo_order, nodes[source].topo_order - nodes[target].topo_order);
            } else {
                return decode_check(*bits, get_p0(nodes[source].p0_pos, pos, connect_p0[nodes[source].topo_order]), (pos+1)*chunk_size,  chunk_size - nodes[target].topo_order%chunk_size);
            }
        }
    }

    std::string ReachabilityCoding::getName() const {
        return "x-Jump";
    }

    std::string ReachabilityCoding::getParams() const {
        return "x=" + std::to_string(chunk_size+1);
    }

    unsigned long long ReachabilityCoding::getIndexSize() const {
        long long index_size = n * sizeof(int) * 2;
        for (int i = 0; i < n; i++) {
            int chunks = (nodes[i].topo_order+chunk_size-1)/chunk_size;
            for (int j = 0; j < chunks; j++) {
                index_size += nodes[i].codes[j].size_bytes;
            }
            index_size += nodes[i].p0_pos.size() * sizeof(Node::pair);
#ifdef DEBUG
            std::cout << nodes[i].topo_order << " " << connect_p0[nodes[i].topo_order].val << ":";
            for (int j = 0; j < get_chunk_num(nodes[i].topo_order); j++)
                std::cout << " " << nodes[i].codes[j].size;
            std::cout << std::endl;
            for (auto pair: nodes[i].p0_pos)
                std::cout << " " << pair.pos << "/" << pair.p0.val;
            std::cout << std::endl;
#endif
        }
        return index_size;
    }

}
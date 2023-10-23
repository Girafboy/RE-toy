#include "ReachCode.h"

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
    ReachCode::ReachCode(int x, int r) : chunk_size(x-1), ratio(r) {}

    ReachCode::FastFloat ReachCode::encode(const Bits &bits, Bits &out, FastFloat p0, int cur, int len) {
        unsigned long long lo = 0, hi = RANGE_MAX, mid;
        int pending = 0;
        for (int i = len-1; i >=0; i--) {
            mid = p0 * (hi - lo);
            if (mid < 1) {
                mid = 1;
            }
            mid += lo;
            if (mid > RANGE_MAX) {
                mid = RANGE_MAX;
            }

            if (bits.get(i)) {
                lo = mid;
                p0 = p0 * connect_p0[cur-len+i];
            } else {
                hi = mid - 1;
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

    void ReachCode::encode(ReachCode::Node &node) {
        int chunks = get_chunk_num(node.topo_order);

        Bits *code_raw = node.codes;

        node.codes = new Bits[chunks];
        FastFloat p0_base = connect_p0[node.topo_order];
        FastFloat p0 = encode(code_raw[chunks-1], node.codes[chunks-1], p0_base, node.topo_order, node.topo_order - chunk_size*(chunks-1));
        for (int i = chunks-2; i >= 0 ; i--) {
            if (approximation_ratio(p0, p0_base) > ratio) {
                p0_base = p0;
                p0_pos[node.topo_order].emplace_back(i, p0_base);
            }
            p0 = p0 * encode(code_raw[i], node.codes[i], p0_base, (i+1)*chunk_size, chunk_size) / p0_base;
        }
        delete [] code_raw;
    }

    void ReachCode::reset() {
        delete [] nodes;
        delete [] connect_p0;
        delete [] p0_pos;
    }

    void ReachCode::construction(const Graph &graph) {
        n = graph.size();
        nodes = new Node[n];
        connect_p0 = new FastFloat[n];
        p0_pos = new std::vector<pair>[n];
        
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

    bool ReachCode::decode_check(const Bits &code, fastfloat_t p0, fastfloat_t *p0_cur, int len) const {
        int size = code.size-1;
        unsigned int value = bswap(*(unsigned int *)code.data);

        int code_cur = 32 - size;
        if (code_cur > 0) {
            value >>= code_cur;
            value <<= code_cur;
        }
        code_cur = 32;

        unsigned long long lo = 0, hi = RANGE_MAX, mid;
        for (len--; len; len--) {
            mid = ((unsigned long long)p0 * (hi - lo)) >> 32;
            if (mid < 1) {
                mid = 1;
            }
            mid += lo;
            if (mid > RANGE_MAX) {
                mid = RANGE_MAX;
            }

            p0_cur--;
            if (value >= mid) {
                lo = mid;
                p0 = (((unsigned long long)p0 * (*p0_cur)) >> 32);
            } else {
                hi = mid - 1;
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
                value <<= 1;
                if (code_cur < size) {
                    value |= code.get(code_cur++);
                }
            }
        }

        mid = ((unsigned long long)p0 * (hi - lo)) >> 32;
        if (mid < 1) {
            mid = 1;
        }
        mid += lo;
        if (mid > RANGE_MAX) {
            mid = RANGE_MAX;
        }
        if (value >= mid) {
            return true;
        } else {
            return false;
        }
    }

    bool ReachCode::TC_haspath(int source, int target) {
        if (source == target) {
            return true;
        }
        int source_topo = nodes[source].topo_order;
        int target_topo = nodes[target].topo_order;
        if (source_topo < target_topo) {
            return false;
        } 
        
        int pos = target_topo / chunk_size;
        Bits *bits = &nodes[source].codes[pos];
        if (bits->get(bits->size-1)) {
            return bits->get(target_topo%chunk_size);
        }
        if (pos == (source_topo - 1) / chunk_size) {
            return decode_check(*bits, get_p0(p0_pos[source_topo], pos, connect_p0[source_topo]).val, (fastfloat_t *) connect_p0 + source_topo, source_topo - target_topo);
        } else {
            return decode_check(*bits, get_p0(p0_pos[source_topo], pos, connect_p0[source_topo]).val, (fastfloat_t *) connect_p0 + (pos+1)*chunk_size,  chunk_size - target_topo%chunk_size);
        }
    }

    std::string ReachCode::getName() const {
        return "x-Jump";
    }

    std::string ReachCode::getParams() const {
        return "x=" + std::to_string(chunk_size+1);
    }

    unsigned long long ReachCode::getIndexSize() const {
        long long index_size = n * sizeof(int) * 2;
        for (int i = 0; i < n; i++) {
            int chunks = (nodes[i].topo_order+chunk_size-1)/chunk_size;
            for (int j = 0; j < chunks; j++) {
                index_size += nodes[i].codes[j].size_bytes;
            }
            index_size += p0_pos[nodes[i].topo_order].size() * sizeof(pair);
#ifdef DEBUG
            std::cout << nodes[i].topo_order << " " << connect_p0[nodes[i].topo_order].val << ":";
            for (int j = 0; j < get_chunk_num(nodes[i].topo_order); j++)
                std::cout << " " << nodes[i].codes[j].size;
            std::cout << "\t\t\t\t";
            for (auto pair: p0_pos[nodes[i].topo_order])
                std::cout << " " << pair.pos << "/" << pair.p0.val;
            std::cout << std::endl;
#endif
        }
        return index_size;
    }

}
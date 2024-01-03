#include "RE_Toy.h"

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iterator>
#include <math.h>
#include <sstream>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>

// #define DEBUG

namespace re {
    RE_Toy::RE_Toy(int x, float r) : chunk_size(x - 1), ratio(r) {}

    unsigned int RE_Toy::encode(Bits &bits, Bits &out, unsigned int p0, int cur, int len) const {
        unsigned long long lo = 0, hi = RANGE_MAX, mid;
        int pending = 0;
        for (int i = len - 1; i >= 0; i--) {
            mid = ((unsigned long long)p0 * (hi - lo)) >> 32;
            if (mid < 1) {
                mid = 1;
            }
            mid += lo;
            if (mid > RANGE_MAX) {
                mid = RANGE_MAX;
            }

            if (bits.get(i)) {
                lo = mid;
                p0 = ((unsigned long long)p0 * parameter[cur - len + i]) >> 32;
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
                hi <<= 1;
                hi |= 1;
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
            out = std::move(bits);
            out.append_one();
        } else {
            out.append_zero();
        }
        return p0;
    }

    void RE_Toy::encode(RE_Toy::Node &node) {
        int chunks = get_chunk_num(node.topo_order);

        Bits *code_raw = node.code.chunks;

        node.code.chunks = new Bits[chunks];
        unsigned int p0_base = parameter[node.topo_order];
        unsigned int p0 = encode(code_raw[chunks - 1], node.code.chunks[chunks - 1], p0_base, node.topo_order,
                              node.topo_order - chunk_size * (chunks - 1));
        for (int i = chunks - 2; i >= 0; i--) {
            if (approximation_ratio(p0, p0_base) > ratio) {
                p0_base = p0;
                node.code.a_state.emplace_back(i, p0_base);
            }
            if (p0_base) {
                p0 = (((unsigned long long)p0 * encode(code_raw[i], node.code.chunks[i], p0_base, (i + 1) * chunk_size, chunk_size)) >> 32) / p0_base;
            } else {
                p0 = encode(code_raw[i], node.code.chunks[i], p0_base, (i + 1) * chunk_size, chunk_size);
            }
        }
        delete[] code_raw;
    }

    void RE_Toy::reset() {
        delete[] nodes;
        delete[] parameter;
    }

    void RE_Toy::construction(const Graph &graph) {
        n = graph.size();
        
        nodes = new Node[n];
        parameter = new unsigned int [n];

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
            int cur = q.front();
            q.pop();
            nodes[cur].topo_order = topo_order++;

            if (nodes[cur].topo_order) {
                int chunks = get_chunk_num(nodes[cur].topo_order);
                nodes[cur].code.chunks = new Bits[chunks];

                for (int i = 0; i < chunks - 1; i++) {
                    nodes[cur].code.chunks[i].init(chunk_size);
                }
                nodes[cur].code.chunks[chunks - 1].init(nodes[cur].topo_order - chunk_size * (chunks - 1));

                for (const int v: graph.getSuccessors(cur)) {
                    nodes[cur].code.chunks[nodes[v].topo_order / chunk_size].set(nodes[v].topo_order % chunk_size);
                    if (nodes[v].topo_order) {
                        int chunks_v = get_chunk_num(nodes[v].topo_order);
                        for (int i = 0; i < chunks_v; i++) {
                            nodes[cur].code.chunks[i].bits_or(nodes[v].code.chunks[i]);
                        }
                        if (--in_degree[v] == 0) {
                            encode(nodes[v]);
                        }
                    }
                }

                unsigned int *a_array = new unsigned int[nodes[cur].topo_order];
                unsigned int a = 0xFFFFFFFFU;
                for (int j = nodes[cur].topo_order-1; j >= 0 ; j--) {
                    a_array[j] = a;
                    if (nodes[cur].code.chunks[j/chunk_size].get(j%chunk_size)) {
                        a = ((unsigned long long)parameter[j] * a) >> 32;
                    }
                }
                unsigned int theta_r = 0x80000000U, step = 0x40000000U;
                for (int i = 0; i < 30; i++) {
                    float df = 0;
                    for (int j = nodes[cur].topo_order-1; j >= 0 ; j--) {
                        if (nodes[cur].code.chunks[j/chunk_size].get(j%chunk_size)) {
                            df += 1.0 / (1.0 / ((float)a_array[j] / (float)0xFFFFFFFF) - ((float)theta_r / (float)0xFFFFFFFF));
                        } else {
                            df += - 1.0 / ((float)theta_r / (float)0xFFFFFFFF);
                        }
                    }
                    if (df > 0) {
                        theta_r -= step;
                    } else {
                        theta_r += step;
                    }
                    step /= 2;
                    // std::cout << theta_r << ' ';
                }
                // std::cout << std::endl;
                parameter[nodes[cur].topo_order] = theta_r;

                if (graph.getInDegree(cur) == 0) {
                    encode(nodes[cur]);
                }
            }

            for (const int u: graph.getPredecessors(cur)) {
                if (--out_degree[u] == 0) {
                    q.push(u);
                }
            }
        }
    }

     bool RE_Toy::decode_check(const Bits &code, fastfloat_t p0, fastfloat_t *p0_cur, int len) {
        int size = code.size - 1;
        unsigned int value = bswap(*(unsigned int *) code.data);

        int code_cur = 32 - size;
        if (code_cur > 0) {
            value >>= code_cur;
            value <<= code_cur;
        }
        code_cur = 32;

        unsigned long long lo = 0, hi = RANGE_MAX, mid;
        for (len--; len; len--) {
            mid = ((unsigned long long) p0 * (hi - lo)) >> 32;
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
                p0 = (((unsigned long long) p0 * (*p0_cur)) >> 32);
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
                hi <<= 1;
                hi |= 1;
                value <<= 1;
                if (code_cur < size) {
                    value |= code.get(code_cur++);
                }
            }
        }

        mid = ((unsigned long long) p0 * (hi - lo)) >> 32;
        if (mid < 1) {
            mid = 1;
        }
        mid += lo;
        if (mid > RANGE_MAX) {
            mid = RANGE_MAX;
        }
        return value >= mid;
    }

    bool RE_Toy::TC_haspath(int source, int target) {
        if (source == target) {
            return true;
        }
        int source_topo = nodes[source].topo_order;
        int target_topo = nodes[target].topo_order;
        if (source_topo < target_topo) {
            return false;
        }

        int pos = target_topo / chunk_size;
        Bits *bits = &nodes[source].code.chunks[pos];
        if (bits->get(bits->size - 1)) {
            return bits->get(target_topo % chunk_size);
        }
        if (pos == (source_topo - 1) / chunk_size) {
            return decode_check(*bits, get_p0(nodes[source].code.a_state, pos, parameter[source_topo]),
                                (fastfloat_t *) parameter + source_topo, source_topo - target_topo);
        } else {
            return decode_check(*bits, get_p0(nodes[source].code.a_state, pos, parameter[source_topo]),
                                (fastfloat_t *) parameter + (pos + 1) * chunk_size,
                                chunk_size - target_topo % chunk_size);
        }
    }

    std::string RE_Toy::getName() const {
        return "ORSE-Toy";
    }

    std::string RE_Toy::getParams() const {
        std::stringstream stream;
        stream << "x=" << chunk_size + 1 << " r=" << std::fixed << std::setprecision(1) << ratio;
        return stream.str();
    }

    unsigned long long RE_Toy::getIndexSize() const {
        unsigned long long index_size = n * sizeof(int) * 2;
        for (int i = 0; i < n; i++) {
            int chunks = (nodes[i].topo_order + chunk_size - 1) / chunk_size;
            for (int j = 0; j < chunks; j++) {
                index_size += nodes[i].code.chunks[j].size_bytes;
            }
            index_size += nodes[i].code.a_state.size() * sizeof(pair);
#ifdef DEBUG
            std::cout << nodes[i].topo_order << " " << parameter[nodes[i].topo_order] << ":";
            for (int j = 0; j < get_chunk_num(nodes[i].topo_order); j++)
                std::cout << " " << nodes[i].code.chunks[j].size;
            std::cout << "\t\t\t\t";
            for (auto pair: nodes[i].code.a_state)
                std::cout << " " << pair.pos << "/" << pair.p0;
            std::cout << std::endl;
#endif
        }
        return index_size;
    }

}
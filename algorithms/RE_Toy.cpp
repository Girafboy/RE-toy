#include "RE_Toy.h"

#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iterator>
#include <math.h>
#include <sstream>
#include <iostream>
#include <queue>
#include <stack>
#include <utility>
#include <vector>
#include <chrono>

// #define DEBUG

namespace re {
    RE_Toy::RE_Toy(int x, float delta) : chunk_size(x - 1), delta(delta) {}

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
            if ((approximation_ratio(p0, p0_base) > delta) || (p0 == 0 && p0_base != 0)) {
                p0_base = p0;
                node.code.a_state.emplace_back(i, p0_base);
            }
            if (p0_base != 0) {
                p0 = (unsigned long long)p0 * encode(code_raw[i], node.code.chunks[i], p0_base, (i + 1) * chunk_size, chunk_size) / p0_base;
            } else {
                encode(code_raw[i], node.code.chunks[i], p0_base, (i + 1) * chunk_size, chunk_size);
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

        std::queue<int> _q;
        std::vector<size_t> _in_degree(n), _out_degree(n);
        std::vector<int> priority(n), _topos(n);
        std::vector<Bits> _bits(n);
        for (int i = 0; i < n; ++i) {
            _in_degree[i] = graph.getInDegree(i);
            _out_degree[i] = graph.getOutDegree(i);
        }
        for (int i = 0; i < n; ++i) {
            if (_in_degree[i] == 0) {
                _q.push(i);
            }
        }
        int _topo_order = 0;
#ifdef DEBUG
        auto _start_time = std::chrono::high_resolution_clock::now();
#endif
        while (!_q.empty()) {
            int cur = _q.front();
            _q.pop();
            _topos[cur] = _topo_order++;
#ifdef DEBUG
            std::cout << _topo_order << "[" << (std::chrono::high_resolution_clock::now() - _start_time).count() / 1000000000 << "]" << '\r';
#endif

            if (_topos[cur]) {
                _bits[cur].init(_topos[cur]);

                for (const int v: graph.getPredecessors(cur)) {
                    if (!_bits[cur].get(_topos[v])) {
                        _bits[cur].set(_topos[v]);
                        if (_topos[v]) {
                            _bits[cur].bits_or(_bits[v]);
                        }
                    }
                    if (_topos[v]) {
                        if (--_out_degree[v] == 0) {
                            delete [] _bits[v].data;
                            _bits[v].data = nullptr;
                        }
                    }
                }

                priority[cur] = _bits[cur].count_ones();
            }

            for (const int u: graph.getSuccessors(cur)) {
                if (--_in_degree[u] == 0) {
                    _q.push(u);
                }
            }
        }

        nodes = new Node[n];
        parameter = new unsigned int [n];

        std::priority_queue<std::pair<int, int>> q;
        std::vector<size_t> out_degree(n), in_degree(n);
        for (int i = 0; i < n; ++i) {
            out_degree[i] = graph.getOutDegree(i);
            in_degree[i] = graph.getInDegree(i);
        }
        for (int i = 0; i < n; ++i) {
            if (out_degree[i] == 0) {
                q.push(std::make_pair(priority[i], i));
            }
        }
        int topo_order = 0;
#ifdef DEBUG
        auto start_time = std::chrono::high_resolution_clock::now();
#endif
        while (!q.empty()) {
            int cur = q.top().second;
            q.pop();
            nodes[cur].topo_order = topo_order++;
#ifdef DEBUG
            std::cout << topo_order << "[" << (std::chrono::high_resolution_clock::now() - start_time).count() / 1000000000 << "]" << '\r';
#endif

            if (nodes[cur].topo_order) {
                int chunks = get_chunk_num(nodes[cur].topo_order);
                nodes[cur].code.chunks = new Bits[chunks];

                for (int i = 0; i < chunks - 1; i++) {
                    nodes[cur].code.chunks[i].init(chunk_size);
                }
                nodes[cur].code.chunks[chunks - 1].init(nodes[cur].topo_order - chunk_size * (chunks - 1));

                for (const int v: graph.getSuccessors(cur)) {
                    if (!nodes[cur].code.chunks[nodes[v].topo_order / chunk_size].get(nodes[v].topo_order % chunk_size)) {
                        nodes[cur].code.chunks[nodes[v].topo_order / chunk_size].set(nodes[v].topo_order % chunk_size);
                        if (nodes[v].topo_order) {
                            int chunks_v = get_chunk_num(nodes[v].topo_order);
                            for (int i = 0; i < chunks_v; i++) {
                                nodes[cur].code.chunks[i].bits_or(nodes[v].code.chunks[i]);
                            }
                        }
                    }
                    if (nodes[v].topo_order) {
                        if (--in_degree[v] == 0) {
                            encode(nodes[v]);
                        }
                    }
                }

                size_t n_ones = 0;
                for (int i = 0; i < chunks; i++) {
                    n_ones += nodes[cur].code.chunks[i].count_ones();
                }
                if (n_ones) {
                    unsigned int *a_array = new unsigned int[n_ones];
                    unsigned int a = 0xFFFFFFFFU, *ptr = a_array;
                    for (int j = nodes[cur].topo_order-1; j >= 0 ; j--) {
                        if (nodes[cur].code.chunks[j/chunk_size].get(j%chunk_size)) {
                            *ptr = a; ptr++;
                            a = ((unsigned long long)parameter[j] * a) >> 32;
                        }
                    }
                    unsigned int theta_r = 0x80000000U, step = 0x40000000U;
                    for (int i = 0; i < 31; i++) {
                        long long df = - (nodes[cur].topo_order - n_ones) << 32;
                        unsigned long long tmp;
                        for (int j = 0; j < n_ones; j++) {
                            tmp = (unsigned long long)a_array[j] * theta_r >> 32;
                            df += tmp * 0xFFFFFFFF / (0xFFFFFFFF - tmp);
                            if ((df > 0) || (df + (unsigned long long)a_array[j] * 0xFFFFFFFF / (0xFFFFFFFF - a_array[j]) * (n_ones - j) < 0))
                                break;
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
                    delete [] a_array;
                    parameter[nodes[cur].topo_order] = theta_r;
                } else {
                    parameter[nodes[cur].topo_order] = 0xFFFFFFFFU;
                }

                if (graph.getInDegree(cur) == 0) {
                    encode(nodes[cur]);
                }
            }

            for (const int u: graph.getPredecessors(cur)) {
                if (--out_degree[u] == 0) {
                    q.push(std::make_pair(priority[u], u));
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
        return "RE-toy";
    }

    std::string RE_Toy::getParams() const {
        std::stringstream stream;
        stream << "x=" << chunk_size + 1 << " delta=" << std::fixed << std::setprecision(1) << delta;
        return stream.str();
    }

    unsigned long long RE_Toy::getIndexSize() const {
        unsigned long long index_size = n * sizeof(int) * 2 * 8;
#ifdef DEBUG
        std::ofstream fout;
        fout.open("log", std::ios_base::out);
        unsigned long long pure_bits_size = 0;
#endif
        for (int i = 0; i < n; i++) {
            int chunks = (nodes[i].topo_order + chunk_size - 1) / chunk_size;
            for (int j = 0; j < chunks; j++) {
                index_size += nodes[i].code.chunks[j].size;
#ifdef DEBUG
                pure_bits_size += nodes[i].code.chunks[j].size;
#endif
            }
            index_size += nodes[i].code.a_state.size() * sizeof(pair) * 8;
#ifdef DEBUG
            fout << nodes[i].topo_order << " " << parameter[nodes[i].topo_order] << ":";
            for (int j = 0; j < get_chunk_num(nodes[i].topo_order); j++)
                fout << " " << nodes[i].code.chunks[j].size;
            fout << "\t\t\t\t";
            for (auto pair: nodes[i].code.a_state)
                fout << " " << pair.pos << "/" << pair.p0;
            fout << "\n";
#endif
        }
#ifdef DEBUG
        fout << pure_bits_size << " / " << index_size << " = " << (float)pure_bits_size/index_size;
        fout.close();
#endif
        return (index_size + 7) / 8;
    }

}
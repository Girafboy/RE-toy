//
// Created by 王星力 on 2022/11/9.
//

#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <vector>

#include "BFL.h"

namespace bfl {
    void BFL::read_graph(const Graph *const graph_ptr) {
        auto n = graph_ptr->size();
        nodes.resize(n);

        for (int u = 0; u < n; u++) {
            nodes[u].N_O_SZ = graph_ptr->getOutDegree(u);
            nodes[u].N_O = new int[nodes[u].N_O_SZ];
            auto successors = graph_ptr->getSuccessors(u);
            copy(successors.begin(), successors.end(), nodes[u].N_O);
            nodes[u].N_I_SZ = graph_ptr->getInDegree(u);
            nodes[u].N_I = new int[nodes[u].N_I_SZ];
            auto predecessors = graph_ptr->getPredecessors(u);
            copy(predecessors.begin(), predecessors.end(), nodes[u].N_I);
        }
    }

    int BFL::h_in() {
        static int c = 0, r = rand();
        if (c >= (int) nodes.size() / D) {
            c = 0;
            r = rand();
        }
        c++;
        return r;
    }

    int BFL::h_out() {
        static int c = 0, r = rand();
        if (c >= (int) nodes.size() / D) {
            c = 0;
            r = rand();
        }
        c++;
        return r;
    }

    void BFL::dfs_in(BFL::node &u) {
        u.vis = vis_cur;

        if (u.N_I_SZ == 0) {
            u.h_in = h_in() % (K * 32);
        } else {
            for (int i = 0; i < K; i++) {
                u.L_in[i] = 0;
            }

            for (int i = 0; i < u.N_I_SZ; i++) {
                node &v = nodes[u.N_I[i]];
                if (v.vis != vis_cur) {
                    dfs_in(v);
                }
                if (v.N_I_SZ == 0) {
                    int hu = v.h_in;
                    u.L_in[(hu >> 5) % K] |= 1 << (hu & 31);
                } else {
                    for (int j = 0; j < K; j++) {
                        u.L_in[j] |= v.L_in[j];
                    }
                }
            }

            int hu = h_in();
            u.L_in[(hu >> 5) % K] |= 1 << (hu & 31);
        }
    }

    void BFL::dfs_out(BFL::node &u) {
        u.vis = vis_cur;

        u.L_interval.first = cur++;

        if (u.N_O_SZ == 0) {
            u.h_out = h_out() % (K * 32);
        } else {
            for (int i = 0; i < K; i++) {
                u.L_out[i] = 0;
            }

            for (int i = 0; i < u.N_O_SZ; i++) {
                node &v = nodes[u.N_O[i]];
                if (v.vis != vis_cur) {
                    dfs_out(v);
                }
                if (v.N_O_SZ == 0) {
                    int hu = v.h_out;
                    u.L_out[(hu >> 5) % K] |= 1 << (hu & 31);
                } else {
                    for (int j = 0; j < K; j++) {
                        u.L_out[j] |= v.L_out[j];
                    }
                }
            }

            int hu = h_out();
            u.L_out[(hu >> 5) % K] |= 1 << (hu & 31);
        }

        u.L_interval.second = cur;
    }

    void BFL::index_construction() {
//        timeval start_at, end_at;
//        gettimeofday(&start_at, 0);

        vis_cur++;
        for (int u = 0; u < nodes.size(); u++) {
            if (nodes[u].N_O_SZ == 0) {
                dfs_in(nodes[u]);
            }
        }
        vis_cur++;
        cur = 0;
        for (int u = 0; u < nodes.size(); u++) {
            if (nodes[u].N_I_SZ == 0) {
                dfs_out(nodes[u]);
            }
        }

//        gettimeofday(&end_at, 0);
//        printf("index time: %.3fs\n",
//               end_at.tv_sec - start_at.tv_sec +
//               double(end_at.tv_usec - start_at.tv_usec) / 1000000);
//        long long index_size = 0;
//        for (int u = 0; u < nodes.size(); u++) {
//            index_size +=
//                    nodes[u].N_I_SZ == 0 ? sizeof(nodes[u].h_in) : sizeof(nodes[u].L_in);
//            index_size +=
//                    nodes[u].N_O_SZ == 0 ? sizeof(nodes[u].h_out) : sizeof(nodes[u].L_out);
//            index_size += sizeof(nodes[u].L_interval);
//        }
//        printf("index space: %.3fMB\n", double(index_size) / (1024 * 1024));
    }

    bool BFL::reach(const BFL::node &u, const BFL::node &v) {
        if (u.L_interval.second < v.L_interval.second) {
            return false;
        } else if (u.L_interval.first <= v.L_interval.first) {
            return true;
        }

        if (v.N_I_SZ == 0) {
            return false;
        }
        if (u.N_O_SZ == 0) {
            return false;
        }
        if (v.N_O_SZ == 0) {
            if ((u.L_out[v.h_out >> 5] & (1 << (v.h_out & 31))) == 0) {
                return false;
            }
        } else {
            for (int i = 0; i < K; i++) {
                if ((u.L_out[i] & v.L_out[i]) != v.L_out[i]) {
                    return false;
                }
            }
        }
        if (u.N_I_SZ == 0) {
            if ((v.L_in[u.h_in >> 5] & (1 << (u.h_in & 31))) == 0) {
                return false;
            }
        } else {
            for (int i = 0; i < K; i++) {
                if ((u.L_in[i] & v.L_in[i]) != u.L_in[i]) {
                    return false;
                }
            }
        }

        for (int i = 0; i < u.N_O_SZ; i++) {
            if (nodes[u.N_O[i]].vis != vis_cur) {
                nodes[u.N_O[i]].vis = vis_cur;
                if (reach(nodes[u.N_O[i]], v)) {
                    return true;
                }
            }
        }

        return false;
    }

    BFL::BFL() {
    }

    std::string BFL::getName() const {
        return "BFL";
    }

    std::string BFL::getParams() const {
        return "s=" + std::to_string(K * 32);
    }

    void BFL::reset() {
        nodes.clear();
        vis_cur = 0;
        cur = 0;
    }

    void BFL::construction(const Graph &graph) {
        read_graph(&graph);
        index_construction();
    }

    bool BFL::TC_haspath(int source, int target) {
        vis_cur++;
        return reach(nodes[source], nodes[target]);
    }

    long long BFL::getIndexSize() const {
        long long index_size = 0;
        for (int u = 0; u < nodes.size(); u++) {
            index_size +=
                    nodes[u].N_I_SZ == 0 ? sizeof(nodes[u].h_in) : sizeof(nodes[u].L_in);
            index_size +=
                    nodes[u].N_O_SZ == 0 ? sizeof(nodes[u].h_out) : sizeof(nodes[u].L_out);
            index_size += sizeof(nodes[u].L_interval);

            /* The author ignors the size of N_O and N_I, which is utilized in BFL::reach. */
            index_size += nodes[u].N_O_SZ * sizeof(*nodes[u].N_O);
            index_size += nodes[u].N_I_SZ * sizeof(*nodes[u].N_I);
            /* The author ignors the size of N_O and N_I, which is utilized in BFL::reach. */
        }
        return index_size;
    }
}
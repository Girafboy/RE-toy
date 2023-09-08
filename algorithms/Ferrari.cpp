#include "Ferrari.h"

#include <numeric>

namespace ferrari {

    Ferrari::Ferrari(unsigned int k, unsigned int seeds, bool global) : k(k), seeds(seeds), global(global), g(nullptr), bm(
            nullptr) {

    }

    std::string Ferrari::getName() const {
        return "Ferrari";
    }

    std::string Ferrari::getParams() const {
        return "k = " + std::to_string(k) + " seeds = " + std::to_string(seeds) + " " + (global ? "global" : "local");
    }

    void Ferrari::construction(const Graph &graph) {
        g = new FerrariGraph(graph);
        bm = new Index(g, seeds, k, global);
        bm->build();
    }

    unsigned long long Ferrari::getIndexSize() const {
        unsigned long long count = 0;
        for (unsigned i = 0; i < g->num_nodes(); ++i) {
            if (bm->get_intervals(i)) {
                count += bm->get_intervals(i)->size();
            }
        }
        unsigned long long interval_space = count * 4 * 2 + count;  // intervals + exactness flag
        unsigned long long seed_space = (bm->used_seed_count() / 8) * 2 * g->num_nodes();
        unsigned long long idspace = g->num_nodes() * 4;
        unsigned long long filter_space = g->num_nodes() * 4 + g->num_nodes() * 4; // top order + top level
        // g->get_neighbors(x)
        unsigned long long graph_neighbor_space = std::accumulate(g->get_degrees()->begin(), g->get_degrees()->end(), 0ull) * sizeof(unsigned);
        return interval_space + seed_space + idspace + filter_space + graph_neighbor_space;
    }

    bool Ferrari::TC_haspath(int source, int target) {
        return bm->reachable(source, target);
    }

    void Ferrari::reset() {
        delete g;
        delete bm;
        g = nullptr;
        bm = nullptr;
    }
}
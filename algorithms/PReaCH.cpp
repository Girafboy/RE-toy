#include "PReaCH.h"
#include "PreachUtils/ConstructionGraph.hh"

namespace preach {
    PReaCH::PReaCH() = default;

    std::string PReaCH::getName() const {
        return "PReaCH";
    }

    std::string PReaCH::getParams() const {
        return {};
    }

    void PReaCH::construction(const Graph &graph) {
        ConstructionGraph cg;
        cg.convert(graph);
        qg = cg.construct();
    }

    void PReaCH::reset() {
        qg.reset();
    }

    bool PReaCH::TC_haspath(int source, int target) {
        return qg->query(source, target);
    }

    unsigned long long PReaCH::getIndexSize() const {
        unsigned long long index_size = 0ull;
        // NodeList nodes;
        index_size += qg->nodes.size() * (2 * sizeof(int) + 2 * (6 * sizeof(int)));
        // Fifo ff;
        index_size += 4 * sizeof(int) + qg->nodes.size() * sizeof(int);
        // std::valarray<int> runs;
//        index_size += qg->runs.size() * sizeof(int);
        // int run
//        index_size += sizeof(int);
        // EdgeSlices fwd_edge_slices;
        index_size += qg->fwd_edge_slices.size() * (2 * sizeof(const int*));
        // EdgeSlices bwd_edge_slices;
        index_size += qg->bwd_edge_slices.size() * (2 * sizeof(const int*));
        // EdgeList fwd_edges;
        index_size += qg->fwd_edges.size() * sizeof(int);
        // EdgeList bwd_edges;
        index_size += qg->bwd_edges.size() * sizeof(int);
        return index_size;
    }
} // preach
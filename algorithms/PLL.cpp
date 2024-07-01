#include "PLL.h"

#include <vector>

namespace pll {
    PLL::PLL(bool use_RQPrunedLandmarkLabeling) : use_RQPrunedLandmarkLabeling(use_RQPrunedLandmarkLabeling) {

    }

    std::string PLL::getName() const {
        return "PLL";
    }

    std::string PLL::getParams() const {
        return use_RQPrunedLandmarkLabeling ? "PLL" : "PPL";
    }

    void PLL::reset() {
        delete rq;
    }

    void PLL::construction(const Graph &graph) {
        std::vector<std::vector<int>> adj(graph.size());
        for (int i = 0; i < graph.size(); ++i) {
            adj[i] = graph.getSuccessors(i);
        }
        if (use_RQPrunedLandmarkLabeling) {
            rq = new RQPrunedLandmarkLabeling(adj);
        } else {
            rq = new RQPrunedPathLabeling(adj);
        }
    }

    bool PLL::TC_haspath(int source, int target) {
        return rq->query(source + 1, target + 1);
    }

    unsigned long long PLL::getIndexSize() const {
        return rq->indexSize();
    }
} // pll
#ifndef TC_FERRARI_H
#define TC_FERRARI_H

#include <string>

#include "Algorithm.h"
#include "../Graph.h"
#include "./FerrariUtils/FerrariGraph.h"
#include "./FerrariUtils/Index.h"

namespace ferrari {
    class Ferrari : public Algorithm {
    private:
        unsigned k;
        unsigned seeds;
        bool global;
        FerrariGraph *g;
        Index *bm;
    public:
        explicit Ferrari(unsigned k = ~0u, unsigned seeds = 0, bool global = true);

        std::string getName() const override;

        std::string getParams() const override;

        void reset() override;

        void construction(const Graph &graph) override;

        bool TC_haspath(int source, int target) override;

        unsigned long long getIndexSize() const override;
    };
}

#endif //TC_FERRARI_H

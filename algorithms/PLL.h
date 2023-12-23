#ifndef TC_PLL_H
#define TC_PLL_H

#include "Graph.h"
#include "Algorithm.h"
#include "PllUtils/PrunedLabeling.h"

namespace pll {

    class PLL : public Algorithm {
    private:
        bool use_RQPrunedLandmarkLabeling;  // else use RQPrunedPathLabeling
        PrunedLabelingBase* rq = nullptr;

    public:
        explicit PLL(bool use_RQPrunedLandmarkLabeling = true);

        std::string getName() const override;

        std::string getParams() const override;

        void reset() override;

        void construction(const Graph &graph) override;

        bool TC_haspath(int source, int target) override;

        unsigned long long getIndexSize() const override;
    };

} // pll

#endif //TC_PLL_H

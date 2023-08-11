#ifndef TC_PREACH_H
#define TC_PREACH_H

#include "Graph.h"
#include "Algorithm.h"
#include "PreachUtils/QueryGraph.hh"

#include <vector>
#include <string>

namespace preach {

    class PReaCH : public Algorithm {
    private:
        std::unique_ptr<QueryGraph> qg;
    public:
        PReaCH();

        std::string getName() const override;

        std::string getParams() const override;

        void reset() override;

        void construction(const Graph &graph) override;

        bool TC_haspath(int source, int target) override;

        unsigned long long getIndexSize() const override;
    };

} // preach

#endif //TC_PREACH_H

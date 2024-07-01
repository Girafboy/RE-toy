#ifndef TC_GRIPPWRAPPER_H
#define TC_GRIPPWRAPPER_H

#include "Algorithm.h"
#include "Graph.h"
#include "GrippUtils/Gripp.h"
#include "GrippUtils/GrippGraph.h"

namespace gripp {

    class GrippWrapper : public Algorithm {
    private:
        Gripp *grp;
    public:
        GrippWrapper();

        std::string getName() const override;

        std::string getParams() const override;

        void reset() override;

        void construction(const Graph &graph) override;

        bool TC_haspath(int source, int target) override;

        unsigned long long getIndexSize() const override;
    };

} // gripp

#endif

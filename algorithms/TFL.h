#ifndef TC_TFL_H
#define TC_TFL_H

#include "Graph.h"
#include "Algorithm.h"

#include <vector>
#include <string>

namespace tfl {

    using namespace ::std;

    class TFL : public Algorithm {
    public:
        TFL();

        std::string getName() const override;

        std::string getParams() const override;

        void reset() override;

        void construction(const Graph &graph) override;

        bool TC_haspath(int source, int target) override;

        unsigned long long getIndexSize() const override;
    };

} // tfl

#endif //TC_TFL_H

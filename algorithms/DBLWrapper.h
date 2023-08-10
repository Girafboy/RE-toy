#ifndef TC_DBLWRAPPER_H
#define TC_DBLWRAPPER_H

#include <string>

#include "Graph.h"
#include "Algorithm.h"
#include "DblUtils/DBL.h"

namespace dbl {

    class DBLWrapper : public Algorithm {
    private:
        DBL dbl;
    public:
        DBLWrapper();

        std::string getName() const override;

        std::string getParams() const override;

        void reset() override;

        void construction(const Graph &graph) override;

        bool TC_haspath(int source, int target) override;

        unsigned long long getIndexSize() const override;
    };

} // dbl

#endif //TC_DBLWRAPPER_H

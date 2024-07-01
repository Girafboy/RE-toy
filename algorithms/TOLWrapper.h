#ifndef TC_TOLWRAPPER_H
#define TC_TOLWRAPPER_H

#include <string>

#include "Algorithm.h"
#include "Graph.h"
#include "TolUtils/utility1.h"

namespace tol {
    using namespace ::std;

    class TOLWrapper : public Algorithm {
    private:
        int style;
        int opr;
        iVector<int> labels;
        int **Index;
        int N = 0;

    public:
        explicit TOLWrapper(int style = 0, int opr = 2);

        string getName() const override;

        string getParams() const override;

        void reset() override;

        void construction(const Graph &graph) override;

        bool TC_haspath(int source, int target) override;

        unsigned long long getIndexSize() const override;
    };

} // tol

#endif

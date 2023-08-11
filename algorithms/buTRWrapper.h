// todo: 待删除
#ifndef TC_BUTRWRAPPER_H
#define TC_BUTRWRAPPER_H

#include "Algorithm.h"
#include "../Graph.h"

namespace butr {
    using namespace ::std;

    class buTRWrapper : public Algorithm {
    public:
        buTRWrapper();

        string getName() const override;

        string getParams() const override;

        void reset() override;

        void construction(const Graph &graph) override;

        bool TC_haspath(int source, int target) override;

        unsigned long long getIndexSize() const override;
    };

} // butr

#endif

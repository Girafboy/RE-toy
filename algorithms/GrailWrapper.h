#ifndef TC_GRAILWRAPPER_H
#define TC_GRAILWRAPPER_H

#include <string>

#include "Algorithm.h"
#include "../Graph.h"
#include "./GrailUtils/Grail.h"

namespace grail {
    using namespace ::std;

    class GrailWrapper : public Algorithm {
    private:
        int LABELINGTYPE = 0;
        bool POOL = false;
        int POOLSIZE = 100;
        int DIM = 2;
        bool LEVEL_FILTER = false;
        int alg_type = 1;

        Grail *grail_ptr;
        GrailGraph g;
        ExceptionList *el;
        int gsize;

    public:
        explicit GrailWrapper(int t = 1, int ltype = 0, int dim = 2);

        string getName() const override;

        string getParams() const override;

        void reset() override;

        void construction(const Graph &graph) override;

        bool TC_haspath(int source, int target) override;

        unsigned long long getIndexSize() const override;
    };

}
#endif

//
// Created by xingliwang on 11/29/22.
//

#ifndef KANG_GRAILWRAPPER_H
#define KANG_GRAILWRAPPER_H

#include <string>

#include "Algorithm.h"
#include "../Graph.h"
#include "./GrailUtils/Grail.h"

namespace grail {
    using namespace ::std;
    class GrailWrapper : public Algorithm {
    private:
        bool SKIPSCC = true;
        bool BIDIRECTIONAL = false;
        int LABELINGTYPE = 0;
        bool UseExceptions = false;
        bool UsePositiveCut = false;
        bool POOL = false;
        int POOLSIZE = 100;
        int DIM = 2;
        int query_num = 100000;
        char *filename = NULL;
        char *testfilename = NULL;
        bool debug = false;
        bool LEVEL_FILTER = false;
        bool LEVEL_FILTER_I = false;
        float labeling_time, query_time, query_timepart, exceptionlist_time;
        int alg_type = 1;

        Grail *grail_ptr;
        GrailGraph g;
        ExceptionList *el;
        int gsize;

    public:
        GrailWrapper(int t = 1, int ltype = 0, int dim = 2);

        string getName() const override;

        string getParams() const override;

        void reset() override;

        void construction(const Graph &graph) override;

        bool TC_haspath(int source, int target) override;

        long long getIndexSize() const override;
    };

}
#endif //KANG_GRAILWRAPPER_H

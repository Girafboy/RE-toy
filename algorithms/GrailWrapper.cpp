#include "GrailWrapper.h"

#include "./GrailUtils/GrailGraph.h"
#include "./GrailUtils/GrailGraphUtil.h"
#include <sys/time.h>
#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <cstring>
#include "./GrailUtils/exception_list.h"
#include "./GrailUtils/exception_list_incremental_base.h"
#include "./GrailUtils/exception_list_incremental_plus.h"

#include "./GrailUtils/utils.h"

namespace grail {
    GrailWrapper::GrailWrapper(int t, int ltype, int dim) : alg_type(t), LABELINGTYPE(ltype), DIM(dim) {

    }

    string GrailWrapper::getName() const {
        return "GRAIL";
    }

    string GrailWrapper::getParams() const {
        return "t=" + to_string(alg_type)
               + " ltype=" + to_string(LABELINGTYPE)
               + " dim=" + to_string(DIM);
    }

    void GrailWrapper::construction(const Graph &graph) {
        g = GrailGraph(graph);

        int left = 0;
        gsize = g.num_vertices();

        GrailGraphUtil::topo_leveler(g);

        grail_ptr = new Grail(g, DIM, LABELINGTYPE, POOL, POOLSIZE);

        grail_ptr->set_level_filter(LEVEL_FILTER);
    }

    bool GrailWrapper::TC_haspath(int source, int target) {
        int s = source, t = target;
        bool r;
        switch (alg_type) {
            case 1:
                r = grail_ptr->reach(s, t, el);
                break;
            case 2:
                r = grail_ptr->reach_lf(s, t, el);
                break;
            case 3:
                r = grail_ptr->bidirectionalReach(s, t, el);
                break;
            case 6:
                r = grail_ptr->bidirectionalReach_lf(s, t, el);
                break;

            case -1:
                r = grail_ptr->reachPP(s, t, el);
                break;
            case -2:
                r = grail_ptr->reachPP_lf(s, t, el);
                break;
            case -3:
                r = grail_ptr->bidirectionalReachPP(s, t, el);
                break;
            case -6:
                r = grail_ptr->bidirectionalReachPP_lf(s, t, el);
                break;
        }
        return r;
    }

    void GrailWrapper::reset() {
        delete grail_ptr;
        delete el;
    }

    unsigned long long GrailWrapper::getIndexSize() const {
        unsigned long long totalIndexSize;
        if (alg_type < 0) {
            totalIndexSize = gsize * DIM * 3;
        } else {
            totalIndexSize = gsize * DIM * 2;
        }
        // visited in Grail
//        totalIndexSize += gsize;
        // g.out_edges, i.e. outList in Grail_GRA
        totalIndexSize += g.num_edges();
        return totalIndexSize * sizeof(int);
    }
}
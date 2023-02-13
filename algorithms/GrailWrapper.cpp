//
// Created by xingliwang on 11/29/22.
//

#include "GrailWrapper.h"

#include "./GrailUtils/GrailGraph.h"
#include "./GrailUtils/GrailGraphUtil.h"
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
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
        return "alg_type=" + to_string(alg_type) + " "
               + "LABELINGTYPE=" + to_string(LABELINGTYPE) + " "
               + "DIM=" + to_string(DIM);
    }

    void GrailWrapper::construction(const Graph &graph) {
        g = GrailGraph(graph);

        int left = 0;
        gsize = g.num_vertices();

        struct timeval after_time, before_time, after_timepart, before_timepart;


//    int *sccmap;
//    if(!SKIPSCC){
//        sccmap = new int[gsize];					// store pair of orignal vertex and corresponding vertex in merged graph
//        vector<int> reverse_topo_sort;
//
//        // merge strongly connected component
//        cout << "merging strongly connected component..." << endl;
//        gettimeofday(&before_time, NULL);
//        GrailGraphUtil::mergeSCC(g, sccmap, reverse_topo_sort);
//        gettimeofday(&after_time, NULL);
//        query_time = (after_time.tv_sec - before_time.tv_sec)*1000.0 +
//                     (after_time.tv_usec - before_time.tv_usec)*1.0/1000.0;
//        cout << "merging time:" << query_time << " (ms)" << endl;
//        cout << "#DAG vertex size:" << g.num_vertices() << "\t#DAG edges size:" << g.num_edges() << endl;
//
////		g.printGraph();
////		ofstream outSCC("scc.out");
////		g.writeGraph(outSCC);
////		outSCC.close();
//    }

        GrailGraphUtil::topo_leveler(g);

        gettimeofday(&before_time, NULL);

        int dimension;
        grail_ptr = new Grail(g, DIM, LABELINGTYPE, POOL, POOLSIZE);

        grail_ptr->set_level_filter(LEVEL_FILTER);
        gettimeofday(&after_time, NULL);

        labeling_time = (after_time.tv_sec - before_time.tv_sec) * 1000.0 +
                        (after_time.tv_usec - before_time.tv_usec) * 1.0 / 1000.0;
//    cout << "#construction time:" << labeling_time << " (ms)" << endl;


        el = NULL;
        exceptionlist_time = 0;
        if (UseExceptions) {
            gettimeofday(&before_time, NULL);
            // el = new ExceptionListIncrementalPlus(g,DIM,LEVEL_FILTER);	 			// filtering by levels doesnot help inside exceptionlists construction
//		cout << "UP TO HERE OK" << endl;
            el = new ExceptionListIncrementalPlus(g, DIM, 0);
            gettimeofday(&after_time, NULL);
            exceptionlist_time = (after_time.tv_sec - before_time.tv_sec) * 1000.0 +
                                 (after_time.tv_usec - before_time.tv_usec) * 1.0 / 1000.0;
            cout << "exceptionlist time:" << exceptionlist_time << " (ms)" << endl;
        }
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
        if (!grail_ptr) {
            delete grail_ptr;
        }
        if (!el) {
            delete el;
        }
    }

    long long GrailWrapper::getIndexSize() const {
        // todo: 未证实
        // int totalIndexSize = gsize * sizeof(GrailVertex);
        // if (alg_type < 0) {
        //     totalIndexSize = gsize * DIM * 3 * sizeof(int);
        // } else {
        //     totalIndexSize = gsize * DIM * 2 * sizeof(int);
        // }
        // if (LEVEL_FILTER) {
        //     totalIndexSize += gsize * sizeof(int);
        // }
        // if (UseExceptions) {
        //     totalIndexSize += el->Size() * sizeof(int);
        // }
        // return totalIndexSize;
        return g.indexSize();
    }
}
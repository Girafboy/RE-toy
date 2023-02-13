//
// Created by 王星力 on 2022/12/4.
//

#include "PathTreeWrapper.h"
#include "PathTreeUtils/PathTreeGraph.h"
#include "PathTreeUtils/PathTreeGraphUtil.h"

namespace path_tree {
    std::string PathTreeWrapper::getName() const {
        return "Path Tree";
    }

    std::string PathTreeWrapper::getParams() const {
        return "alg_type=" + std::to_string(alg_type);
    }

    void PathTreeWrapper::construction(const Graph &graph) {
        g = PathTreeGraph(graph);

        int left = 0;
        int gsize = g.num_vertices();

        bool r;
        struct timeval after_time, before_time;
        float labeling_time, query_time;
        sccmap = new int[gsize];    // store pair of orignal vertex and corresponding vertex in merged graph
        vector<int> reverse_topo_sort;

        PathTreeGraphUtil::mergeSCC(g, sccmap, reverse_topo_sort);

        pt_ptr = new PathTree(g, reverse_topo_sort);

        pt_ptr->compute_tcm();

        pt_ptr->createLabels(alg_type);
    }

    void PathTreeWrapper::reset() {
        if (!pt_ptr) {
            delete pt_ptr;
        }
        if (!sccmap) {
            delete[]sccmap;
        }
    }

    PathTreeWrapper::PathTreeWrapper(int alg_type) : alg_type(alg_type) {

    }

    bool PathTreeWrapper::TC_haspath(int source, int target) {
        int s, t;
        s = sccmap[source];
        t = sccmap[target];
        return pt_ptr->reach(s, t);
    }

    long long PathTreeWrapper::getIndexSize() const {
        int *ind_size = new int[2];
        pt_ptr->index_size(ind_size);
        long long ans = ind_size[0] * sizeof(int);
        delete[]ind_size;
        return ans;
    }
}
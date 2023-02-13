/* Copyright (c) Hilmi Yildirim 2010,2011.

The software is provided on an as is basis for research purposes.
There is no additional support offered, nor are the author(s) 
or their institutions liable under any circumstances.
*/
#ifndef _GRAIL_GRAPH_UTIL_H_
#define _GRAIL_GRAPH_UTIL_H_

#include "GrailGraph.h"
#include <sys/time.h>

namespace grail {
    class GrailGraphUtil {
    public:
        static void dfs(GrailGraph &g, int vid, vector<int> &preorder, vector<int> &postorder, vector<bool> &visited);

        static void topo_leveler(GrailGraph &g);

        static int topo_level(GrailGraph &g, int vid);

        static void topological_sort(GrailGraph g, vector<int> &ts);

        static void tarjan(GrailGraph &g, int vid, int &index, hash_map<int, pair<int, int> > &order, vector<int> &sn,
                           multimap<int, int> &sccmap, int &scc);

        static void mergeSCC(GrailGraph &g, int *on, vector<int> &ts);

        static void traverse(GrailGraph &tree, int vid, int &pre_post, vector<bool> &visited);

        static void pre_post_labeling(GrailGraph &tree);

        static void genRandomGraph(int n, double c, char *filename);
    };
}
#endif

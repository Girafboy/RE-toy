#ifndef _PATH_TREE_H
#define _PATH_TREE_H

#include "PathTreeGraphUtil.h"
#include "DWGraphUtil.h"

namespace path_tree {
// test switch
//#define _TEST_

    class PathTree {
    public:
        PathTreeGraph &g;
        DWGraph pg;
        PathTreeGraph ng;    // equivalent weight graph
        DWGraph branch;
        PathTreeGraph newbranch;

        vector<int> nextVertex;
        vector<set<int> > out_uncover;
        vector<vector<int> > pathMap;
        vector<int> grts;    // graph reverse topological sort
        int **labels;

        map<pair<int, int>, bool> tcm;    // for test
        struct timeval after_time, before_time;
        float run_time;

    public:
        PathTree(PathTreeGraph &graph);

        PathTree(PathTreeGraph &graph, vector<int> ts);

        ~PathTree();

        void buildWeightPathGraph();

        void buildWeightPathGraph_Pred();    // update weight by pred size
        void buildEquGraph();

        void createLabels(int type);

        void displayLabels();

        //	void pathDFS(int vid, int& order, int& first_order);
        void pathDFS(int vid, int &order, int &first_order, vector<bool> &visited);

        void transform(DWGraph dg, PathTreeGraph &graph);

        void compute_tcm();

        bool reach(int src, int trg);

        bool test_reach(int src, int trg);

        void index_size(int *ind_size);

        void insertSet(set<int> &s1, set<int> &s2);
    };
}
#endif

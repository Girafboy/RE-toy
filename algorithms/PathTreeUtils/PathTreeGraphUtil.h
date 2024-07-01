#ifndef _PATH_TREE_GRAPH_UTIL_H_
#define _PATH_TREE_GRAPH_UTIL_H_

#include "PathTreeGraph.h"
#include "DWGraph.h"
#include <sys/time.h>

namespace path_tree {

    class PathTreeGraphUtil {
    public:
        static void
        dfs(PathTreeGraph &g, int vid, vector<int> &preorder, vector<int> &postorder, vector<bool> &visited);

        static void topological_sort(PathTreeGraph g, vector<int> &ts);

        static void transitive_closure(PathTreeGraph g, PathTreeGraph &tc);

        static void
        tarjan(PathTreeGraph &g, int vid, int &index, hash_map<int, pair<int, int> > &order, vector<int> &sn,
               multimap<int, int> &sccmap, int &scc);

        static void mergeSCC(PathTreeGraph &g, int *on, vector<int> &ts);

        static void findTreeCover(PathTreeGraph g, PathTreeGraph &tree);

        static void findTreeCover(PathTreeGraph g, PathTreeGraph &tree, vector<set<int> > &pred);

        static void findTreeCover(PathTreeGraph g, PathTreeGraph &tree, vector<set<int> > &pred, vector<int> ts);

        static void compute_pred(PathTreeGraph g, vector<set<int> > &predMap);

        static void findTreeCoverL(PathTreeGraph g, PathTreeGraph &tree);

        static void traverse(PathTreeGraph &tree, int vid, int &pre_post, vector<bool> &visited);

        static void pre_post_labeling(PathTreeGraph &tree);

        static void pathDecomposition(PathTreeGraph &g, vector<vector<int> > &pathMap);

        static void pathDecomposition(PathTreeGraph &g, vector<vector<int> > &pathMap, vector<int> ts);

        static void treePathDecomposition(PathTreeGraph tree, PathTreeGraph &g, vector<vector<int> > &pathMap);

        static void genRandomGraph(int n, double c, char *filename);
    };
}
#endif

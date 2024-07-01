#ifndef _PATH_TREE_GRAPH_H
#define _PATH_TREE_GRAPH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <deque>
#include <algorithm>
#include <utility>
#include <cmath>
#include <ext/hash_map>
#include "Graph.h"
//#include <hash_map.h>

namespace path_tree {
//using namespace std;
//using namespace __gnu_cxx;
//namespace std {using namespace __gnu_cxx;}
    using namespace ::std;
    using namespace ::__gnu_cxx;

#define MAX_VAL 10000000
#define MIN_VAL -10000000

    struct PathTreeVertex {
        bool visited;
        bool fat;    // fat node
        int topo_id;    // topological order
        int path_id;    // path id
        int dfs_order;
        int pre_order;
        int post_order;
        int first_visit; // for test
    };

    typedef vector<int> PathTreeEdgeList;    // edge list represented by vertex id list
    typedef vector<PathTreeVertex> PathTreeVertexList;    // vertices list (store real vertex property) indexing by id

    struct PathTree_In_OutList {
        PathTreeEdgeList inList;
        PathTreeEdgeList outList;
    };
    typedef vector<PathTree_In_OutList> Path_GRA;    // index graph

    class PathTreeGraph {
    private:
        Path_GRA graph;
        PathTreeVertexList vl;
        int vsize;

    public:
        PathTreeGraph();

        PathTreeGraph(int);

        PathTreeGraph(Path_GRA &, PathTreeVertexList &);

        PathTreeGraph(istream &);

        PathTreeGraph(const Graph &input_graph);

        ~PathTreeGraph();

        void readGraph(istream &);

        void convertGraph(const Graph &input_graph);

        void writeGraph(ostream &);

        void printGraph();

        void addVertex(int);

        void addEdge(int, int);

        int num_vertices();

        int num_edges();

        PathTreeVertexList &vertices();

        PathTreeEdgeList &out_edges(int);

        PathTreeEdgeList &in_edges(int);

        int out_degree(int);

        int in_degree(int);

        vector<int> getRoots();

        bool hasEdge(int, int);

        PathTreeGraph &operator=(const PathTreeGraph &);

        PathTreeVertex &operator[](const int);

        PathTreeGraph(hash_map<int, vector<int> > &inlist, hash_map<int, vector<int> > &outlist);

        void extract(hash_map<int, vector<int> > &inlist, hash_map<int, vector<int> > &outlist);

        void printMap(hash_map<int, vector<int> > &inlist, hash_map<int, vector<int> > &outlist);

    };
}
#endif

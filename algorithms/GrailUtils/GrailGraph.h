/* Copyright (c) Hilmi Yildirim 2010,2011.

The software is provided on an as is basis for research purposes.
There is no additional support offered, nor are the author(s) 
or their institutions liable under any circumstances.
*/
#ifndef _GRAIL_GRAPH_H
#define _GRAIL_GRAPH_H

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

#include "../../Graph.h"

namespace grail {

//    namespace std { using namespace __gnu_cxx; }
    using namespace ::std;
    using __gnu_cxx::hash_map;

#define MAX_VAL 10000000
#define MIN_VAL -10000000

    struct GrailVertex {
        int id;
        int top_level;    // topological level
        int min_parent_level;    // level of the highest parent in top_order
        int topo_id;    // topological order
        int min_int;
        long volume;
        double adj_vol;
        bool visited;
        int dfs_order;
        int pre_order;
        int post_order;
        double tcs;
        int mingap;
        vector<int> *pre;
        vector<int> *post;
        vector<int> *middle;

        GrailVertex(int ID) : id(ID) {
            top_level = -1;
            visited = false;
        }

        GrailVertex() {
            top_level = -1;
            visited = false;
        };
    };

    struct VertexCompare {
        bool operator()(const GrailVertex p1, const GrailVertex p2) const {
            return p1.id < p2.id;
        }
    };

    typedef vector<int> GrailEdgeList;    // edge list represented by vertex id list
    typedef vector<GrailVertex> GrailVertexList;    // vertices list (store real vertex property) indexing by id

    struct Grail_In_OutList {
        GrailEdgeList inList;
        GrailEdgeList outList;
    };
    typedef vector<Grail_In_OutList> Grail_GRA;    // index graph

    class GrailGraph {
    private:
        Grail_GRA graph;
        GrailVertexList vl;
        int vsize;

    public:
        GrailGraph();

        GrailGraph(int);

        GrailGraph(Grail_GRA &, GrailVertexList &);

        GrailGraph(istream &);

        GrailGraph(const Graph &graph);

        ~GrailGraph();

        bool contains(int src, int trg, int dim);

        bool incrementalContains(int src, int trg, int cur);

        void readGraph(istream &);

        void convertGraph(const Graph &input_graph);

        void writeGraph(ostream &);

        void printGraph();

        void addVertex(int);

        void addEdge(int, int);

        int num_vertices();

        int num_edges();

        GrailVertexList &vertices();

        GrailEdgeList &out_edges(int);

        GrailEdgeList &in_edges(int);

        int out_degree(int);

        int in_degree(int);

        vector<int> getRoots();

        bool hasEdge(int, int);

        GrailGraph &operator=(const GrailGraph &);

        GrailVertex &operator[](const int);

        const double actualgap(const int);

        const double tcs(const int);

        void clear();

        void strTrimRight(string &str);

        GrailGraph(hash_map<int, vector<int> > &inlist, hash_map<int, vector<int> > &outlist);

        void extract(hash_map<int, vector<int> > &inlist, hash_map<int, vector<int> > &outlist);

        void printMap(hash_map<int, vector<int> > &inlist, hash_map<int, vector<int> > &outlist);

    };
}
#endif

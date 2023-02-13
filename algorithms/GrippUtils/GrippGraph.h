//
// Created by 王星力 on 2022/12/27.
//

#ifndef XJUMP_GRIPPGRAPH_H
#define XJUMP_GRIPPGRAPH_H

#include"Util.h"
#include "../Graph.h"

namespace gripp {
    using namespace ::std;
    class GrippGraph
    {
    public:
        GrippGraph(int maxn = 0);
        ~GrippGraph();
        void load(const char *filename);
        void convert(const Graph &graph);
        unsigned getN();
        unsigned getM();
        unsigned getMP(unsigned val);
        unsigned GetDegree(unsigned node);
        vector<unsigned>* GetChild(unsigned node);
        bool hasGreaterDegree(unsigned u,unsigned v);
        bool CanReach(unsigned nodeA,unsigned nodeB);
        void disp();
    private:
        unsigned node_num;
        unsigned edge_num;
        vector<vector<unsigned> >	*adj_listp;
        vector<unsigned>  *degreep;
        map<unsigned,unsigned> mp;
        bool search(unsigned nodeA,unsigned nodeB);
    };
}

#endif //XJUMP_GRIPPGRAPH_H

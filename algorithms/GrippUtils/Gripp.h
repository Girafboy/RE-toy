//
// Created by 王星力 on 2022/12/27.
//

#ifndef XJUMP_GRIPP_H
#define XJUMP_GRIPP_H

#include "Util.h"
#include "GrippGraph.h"

namespace gripp {

    using namespace ::std;

    class Gripp
    {
    public:
        Gripp(int maxn);
        ~Gripp();
        int LoadGraph(GrippGraph *gp);
        item *GetItem(unsigned id);
        unsigned GetTreeId(unsigned node);
        list<unsigned> GetHopNodes(unsigned node);
        bool CanReach(unsigned nodeA,unsigned nodeB);
        void display();
        long long getIndexSize() const;
    private:
        GrippGraph * graphp;
        unsigned entry_num;
        unsigned node_num;
        vector<item>* Ind_table_p;
        vector<list<unsigned> >* Inst_p;
        bool vis[Util::MAX_NODE];
        vector<bool> used_hop;
        void dfs(unsigned node);
        bool search(unsigned nodeA,unsigned nodeB);
    };

} // gripp

#endif //XJUMP_GRIPP_H

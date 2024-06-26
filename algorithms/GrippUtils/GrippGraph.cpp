#include "GrippGraph.h"

namespace gripp {
    using namespace ::std;

    static bool vis[Util::MAX_NODE];
    GrippGraph::GrippGraph(int maxn)
    {
        //TODO: use this->adj_listp directly
        //vector<vector<unsigned> > *adj_listp = NULL;
        this->adj_listp = NULL;
        adj_listp = new vector<vector<unsigned> >(maxn);
//	cerr<<adj_listp<<endl;
        if(adj_listp == NULL)
        {
            cerr<<"error to alloc for adj_listp"<<endl;
        }
//	cerr<<adj_listp->size()<<endl;
//	cerr<<(*adj_listp)[0].size()<<endl;
//	cerr<<(*adj_listp)[1].size()<<endl;
//	cerr<<(*adj_listp)[2].size()<<endl;
//	cerr<<(*adj_listp)[10000000].size()<<endl;
        //vector<unsigned> *degreep = NULL;
        this->degreep = NULL;
        degreep = new vector<unsigned>(maxn, 0);
//	cerr<<degreep<<endl;
        if(degreep == NULL)
        {
            cerr<<"error to alloc for degreep"<<endl;
        }
        //cerr<<"check again: "<<adj_listp->size()<<endl;
    }
    GrippGraph::~GrippGraph()
    {
        delete adj_listp;
        delete degreep;
    }
    void GrippGraph::load(const char *filename)
    {
        //cerr<<"start: "<<this->adj_listp->size()<<endl;
        ifstream fin;
        fin.open(filename);
        if(fin.fail())
        {
            cout << "open file failed" << endl;
            //system("pause");
            exit(0);
        }
        char op,tmp;
        int u,v,l;
        bool firstE = false;
        while(1)
        {
            fin >> op;
            if(op == 't')
            {
                fin >> tmp >> l;
                if(l == -1)
                {
                    fin.close();
                    return;
                }
            }
            else if(op == 'v')
            {
                fin >> u >> l;
                mp[u] = 0;
            }
            else if(op == 'e')
            {
                if(!firstE)
                {
                    /* map all nodes into label 1 to node_num */
                    firstE = true;
                    int cnt = 1;
                    for(auto iter = mp.begin();iter != mp.end();iter++)
                    {
                        iter->second = cnt++;
                    }
                    node_num = cnt-1;
                    edge_num = 0;
                }
                fin >> u >> v >> l;
                u = mp[u];
                v = mp[v];
                //(*adj_listp)[u].push_back(v);
                //cerr<<"check: "<<adj_listp->size()<<endl;
                vector<unsigned>& tmp = (*adj_listp)[u];
                //cerr<<"check: "<<tmp.size()<<endl;
                tmp.push_back(v);
                (*degreep)[u]++;
                edge_num++;
            }
        }
    }
    unsigned GrippGraph::getN()
    {
        return node_num;
    }
    unsigned GrippGraph::getM()
    {
        return edge_num;
    }
    unsigned GrippGraph::getMP(unsigned val)
    {
        return mp[val];
    }
    unsigned GrippGraph::GetDegree(unsigned node)
    {
        return (*degreep)[node];
    }
    vector<unsigned>* GrippGraph::GetChild(unsigned node)
    {
        //return (*adj_listp)[node];
        vector<unsigned>* ret = new vector<unsigned>((*adj_listp)[node]);
        return ret;
    }
    bool GrippGraph::hasGreaterDegree(unsigned u, unsigned v)
    {
        return (*degreep)[u] > (*degreep)[v];
    }
    bool GrippGraph::CanReach(unsigned nodeA, unsigned nodeB)
    {
        /*
        nodeA = mp[nodeA];
        nodeB = mp[nodeB];
        */
        nodeA = nodeA + 1;
        nodeB = nodeB + 1;
        memset(vis,0,sizeof(vis));
        return search(nodeA,nodeB);
    }
    bool GrippGraph::search(unsigned nodeA, unsigned nodeB)
    {
        if(nodeA == nodeB) return true;
        vis[nodeA] = true;
        int len = (*adj_listp)[nodeA].size();
        for(int i = 0;i < len; i++)
        {
            if(!vis[(*adj_listp)[nodeA][i]] &&
               search((*adj_listp)[nodeA][i],nodeB))
                return true;
        }
        return false;
    }
    void GrippGraph::disp()
    {
        cout<<"load graph done"<<endl;
        cout<<"n = "<<node_num<<" m = "<<edge_num<<endl;
    }

    void GrippGraph::convert(const Graph &graph) {
        for (int u = 0; u < graph.size(); ++u) {
            mp[u] = 0;
        }
        int cnt = 1;
        for(auto iter = mp.begin();iter != mp.end();iter++)
        {
            iter->second = cnt++;
        }
        node_num = cnt-1;
        edge_num = 0;
        for (int u = 0; u < graph.size(); ++u) {
            for (auto v : graph.getSuccessors(u)) {
                int uu = mp[u];
                int vv = mp[v];
                vector<unsigned>& tmp = (*adj_listp)[uu];
                tmp.push_back(vv);
                (*degreep)[uu]++;
                edge_num++;
            }
        }
    }
}
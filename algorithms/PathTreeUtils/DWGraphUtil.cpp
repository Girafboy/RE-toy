#include "DWGraphUtil.h"

namespace path_tree {
// implement tarjan's algorithm to find Strongly Connected Component from a given start node
    void DWGraphUtil::tarjan(DWGraph &g, int vid, int &index, map<int, pair<int, int> > &order,
                             vector<int> &sn, map<int, vector<int> > &sccmap, int &scc) {
        order[vid].first = index;
        order[vid].second = index;
        index++;
        sn.push_back(vid);
        g.vl[vid].visited = true;
        DWEdgeList el = g.out_edges(vid);
        DWEdgeList::iterator eit;
        for (eit = el.begin(); eit != el.end(); eit++) {
            if (!g.vl[g.target(*eit)].visited) {
                tarjan(g, g.target(*eit), index, order, sn, sccmap, scc);
                order[vid].second = min(order[g.target(*eit)].second, order[vid].second);
            } else if (find(sn.begin(), sn.end(), g.target(*eit)) != sn.end()) {
                order[vid].second = min(order[g.target(*eit)].first, order[vid].second);
            }
        }

        vector<int>::reverse_iterator rit;
        if (order[vid].first == order[vid].second) {
            vector<int> vec;
            for (rit = sn.rbegin(); rit != sn.rend(); rit++) {
                if ((*rit) != vid) {
                    vec.push_back(*rit);
                    sn.pop_back();
                } else {
                    vec.push_back(*rit);
                    sn.pop_back();
                    break;
                }
            }
            sccmap.insert(make_pair(scc, vec));
            scc++;
        }
    }

    void DWGraphUtil::findMaxBranching1(DWGraph &g, DWGraph &maxBranch) {
        DWVertexList::iterator vlit;
        DWVertexList vl = g.vertices();
        DWEdgeList el;
        DWEdgeList::iterator eit, eit1;

        // for test
//	cout << "read graph" << endl;
//	g.printGraph();
        static int branch_depth = 0;
        branch_depth++;

        int maxw = MIN_VAL;
        DWVertexProp maxe;
        for (vlit = vl.begin(); vlit != vl.end(); vlit++) {
            maxw = MIN_VAL;
            el = g.in_edges(vlit->first);
            for (eit = el.begin(); eit != el.end(); eit++) {
                if (g.edgeOpMap[*eit].weight > maxw) {
                    maxw = g.edgeOpMap[*eit].weight;
                    maxe.id = g.edgeOpMap[*eit].src;
                    maxe.edgeid = *eit;
                    maxe.weight = maxw;
                }
            }
            if (maxw != MIN_VAL) {
                maxBranch.addEdge(maxe.id, vlit->first, maxw, maxe.edgeid);
            } else
                maxBranch.addVertex(vlit->first);
        }

        // for test
/*
	cout << "max branch" << endl;
	maxBranch.printGraph();
*/
        // find strongly connected component
        vector<int> list_s;
        int index = 0;
        vector<int> sn;
        vector<int>::iterator vit;
        vector<int>::reverse_iterator rvit;
        map<int, pair<int, int> > order;
        map<int, vector<int> > sccmap;
        int scc_num = 0;
        int vid;
        vl = maxBranch.vertices();
        for (vlit = vl.begin(); vlit != vl.end(); vlit++) {
            vid = vlit->first;
            if (maxBranch.vl[vid].visited) continue;
            tarjan(maxBranch, vid, index, order, sn, sccmap, scc_num);
        }

//	cout << "scc_num = " << scc_num;

        if (scc_num == maxBranch.num_vertices())
            return;

        map<int, vector<int> >::iterator mit = sccmap.begin();
        int num_comp;
        for (mit = sccmap.begin(); mit != sccmap.end(); mit++) {
            num_comp = mit->first;

            //	cout << "scc " << num_comp << " size=" << mit->second.size() << endl;

            if (mit->second.size() == 1)
                continue;

            vector<int> sccvec = mit->second;
            // find min weight edge in sccvec
            int minw = MAX_VAL;
            int weight, edgeid, src, trg;
            for (rvit = sccvec.rbegin(); rvit != sccvec.rend(); rvit++) {
                if ((rvit + 1) != sccvec.rend()) {
                    weight = maxBranch.weight(*rvit, *(rvit + 1));
                    edgeid = maxBranch.edgeId(*rvit, *(rvit + 1));
                    src = *rvit;
                    trg = *(rvit + 1);
                } else {
                    weight = maxBranch.weight(*rvit, sccvec.back());
                    edgeid = maxBranch.edgeId(*rvit, sccvec.back());
                    src = *rvit;
                    trg = sccvec.back();
                }
                if (weight < minw) {
                    minw = weight;
                    maxBranch.removeEdgeWithID(src, trg, edgeid);
                }
            }
        }
    }

// Edmonds' Branching Algorithm
    void DWGraphUtil::findMaxBranching(DWGraph &g, DWGraph &maxBranch) {
        DWVertexList::iterator vlit;
        DWVertexList vl = g.vertices();
        DWEdgeList el;
        DWEdgeList::iterator eit, eit1;

        // for test
//	cout << "read graph" << endl;
//	g.printGraph();
        static int branch_depth = 0;
        branch_depth++;

        int maxw = MIN_VAL;
        DWVertexProp maxe;
        for (vlit = vl.begin(); vlit != vl.end(); vlit++) {
            maxw = MIN_VAL;
            el = g.in_edges(vlit->first);
            for (eit = el.begin(); eit != el.end(); eit++) {
                if (g.edgeOpMap[*eit].weight > maxw) {
                    maxw = g.edgeOpMap[*eit].weight;
                    maxe.id = g.edgeOpMap[*eit].src;
                    maxe.edgeid = *eit;
                    maxe.weight = maxw;
                }
            }
            if (maxw != MIN_VAL) {
                maxBranch.addEdge(maxe.id, vlit->first, maxw, maxe.edgeid);
            } else
                maxBranch.addVertex(vlit->first);
        }

        // for test
/*
	cout << "max branch" << endl;
	maxBranch.printGraph();
*/
        // find strongly connected component
        vector<int> list_s;
        int index = 0;
        vector<int> sn;
        vector<int>::iterator vit;
        vector<int>::reverse_iterator rvit;
        map<int, pair<int, int> > order;
        map<int, vector<int> > sccmap;
        int scc_num = 0;
        int vid;
        vl = maxBranch.vertices();
        for (vlit = vl.begin(); vlit != vl.end(); vlit++) {
            vid = vlit->first;
            if (maxBranch.vl[vid].visited) continue;
            tarjan(maxBranch, vid, index, order, sn, sccmap, scc_num);
        }

//	cout << "scc_num = " << scc_num;

        if (scc_num == maxBranch.num_vertices())
            return;

        DWGraph ng = g;
//	ng.printGraph();


        map<int, vector<int> >::iterator mit = sccmap.begin();
        int num_comp;
        int maxid = ng.maxid();
        EdgeMap edgemap;
        map<int, vector<int> > newVertex;
        map<int, int> minWeight;
        map<int, DWVertexProp> sccPropMap;    // keep edge info in strongly connceted component, <src, trgEdgeProp>
        DWEdgeList inedge, outedge;

        // for test
        bool del = false;

        for (mit = sccmap.begin(); mit != sccmap.end(); mit++) {
            num_comp = mit->first;

            //	cout << "scc " << num_comp << " size=" << mit->second.size() << endl;

            if (mit->second.size() == 1)
                continue;

            // add new vertex
            maxid++;
            ng.addVertex(maxid);
            list_s.push_back(maxid);
            newVertex.insert(make_pair(maxid, mit->second));
            vector<int> sccvec = mit->second;

            // find min weight edge in sccvec
            int minw = MAX_VAL;
            int weight, edgeid;
            for (rvit = sccvec.rbegin(); rvit != sccvec.rend(); rvit++) {
                DWVertexProp prop;
                if ((rvit + 1) != sccvec.rend()) {
                    weight = maxBranch.weight(*rvit, *(rvit + 1));
                    edgeid = maxBranch.edgeId(*rvit, *(rvit + 1));
                    prop.id = *(rvit + 1);
                    prop.weight = weight;
                    prop.edgeid = edgeid;
                    sccPropMap.insert(make_pair(*rvit, prop));
                } else {
                    weight = maxBranch.weight(*rvit, sccvec.back());
                    edgeid = maxBranch.edgeId(*rvit, sccvec.back());
                    prop.id = sccvec.back();
                    prop.weight = weight;
                    prop.edgeid = edgeid;
                    sccPropMap.insert(make_pair(*rvit, prop));
                }
                if (weight < minw)
                    minw = weight;
            }
            minWeight[maxid] = minw;

            for (vit = sccvec.begin(); vit != sccvec.end(); vit++) {
                // process out edges
                el = ng.out_edges(*vit);
                for (eit = el.begin(); eit != el.end(); eit++) {
                    if (find(sccvec.begin(), sccvec.end(), g.target(*eit)) != sccvec.end())
                        continue;
                    ng.addEdge(maxid, g.target(*eit), g.edgeOpMap[*eit].weight, *eit);
                    DWVertexProp newnode;
                    newnode.id = g.target(*eit);
                    newnode.weight = g.edgeOpMap[*eit].weight;
                    newnode.edgeid = *eit;
                    edgemap.insert(make_pair(make_pair(maxid, newnode), make_pair(*vit, newnode)));
                }

                // find edge whose target is vit
                inedge = maxBranch.in_edges(*vit);
                int ce = g.edgeOpMap[*(inedge.begin())].weight;
                // process in edges;
                el = ng.in_edges(*vit);
                for (eit = el.begin(); eit != el.end(); eit++) {
                    if (find(sccvec.begin(), sccvec.end(), g.target(*eit)) != sccvec.end())
                        continue;
                    // update weight
                    int y1 = ng.edgeOpMap[*eit].weight - ce + minw;
                    ng.addEdge(ng.edgeOpMap[*eit].trg, maxid, y1, *eit);
                    DWVertexProp src, trg;
                    src.id = maxid;
                    src.weight = y1;
                    src.edgeid = *eit;
                    trg.id = *vit;
                    trg.weight = ng.edgeOpMap[*eit].weight;
                    trg.edgeid = *eit;
                    edgemap.insert(
                            make_pair(make_pair(ng.edgeOpMap[*eit].trg, src), make_pair(ng.edgeOpMap[*eit].trg, trg)));
                }
                ng.removeVertexfromVL(*vit);
            }
        }

        //for test
/*
	cout << "before return " << endl;
	ng.printGraph();
*/

        // find max branching recursively
        DWGraph newBranch;
        findMaxBranching(ng, newBranch);
        maxBranch = newBranch;

        // for test
/*
	cout << "recursive " << endl;
	maxBranch.printGraph();
*/

        // cycle decomposition
        vector<int>::iterator vit1;
        vector<int>::reverse_iterator rvit1, rvit2;
        vector<int> vec1;
        int minsrc, mintrg;
        bool keep = false;
        for (rvit2 = list_s.rbegin(); rvit2 != list_s.rend(); rvit2++) {
            vec1 = newVertex[*rvit2];
            for (vit1 = vec1.begin(); vit1 != vec1.end(); vit1++)
                maxBranch.addVertex(*vit1);


            //update in edges and strongly connected component
            if (maxBranch.in_edges(*rvit2).size() == 0) {
                keep = false;
                for (rvit1 = vec1.rbegin(); rvit1 != vec1.rend(); rvit1++) {
                    DWVertexProp dwep = sccPropMap[*rvit1];
                    if (dwep.weight == minWeight[*rvit2] && !keep) {
                        keep = true;
                        continue;
                    }
                    maxBranch.addEdge(*rvit1, dwep.id, dwep.weight, dwep.edgeid);
                }
            } else {
                el = maxBranch.in_edges(*rvit2);
                DWVertexProp dwep2 = maxBranch.edge(g.target(*(el.begin())), *rvit2);
                Edge edge = make_pair(g.target(*(el.begin())), dwep2);
                EdgeMap::iterator newedge = edgemap.find(edge);
                DWVertexProp dwep1 = newedge->second.second;
                int src = newedge->second.first;
                maxBranch.addEdge(src, dwep1.id, dwep1.weight, dwep1.edgeid);
                keep = false;
                for (rvit1 = vec1.rbegin(); rvit1 != vec1.rend(); rvit1++) {
                    DWVertexProp dwep = sccPropMap[*rvit1];
                    if (dwep.id == dwep1.id && !keep) {
                        keep = true;
                        continue;
                    }
                    maxBranch.addEdge(*rvit1, dwep.id, dwep.weight, dwep.edgeid);
                }
            }

            // update out edges
            if (maxBranch.out_edges(*rvit2).size() > 0) {
                el = maxBranch.out_edges(*rvit2);
                for (eit = el.begin(); eit != el.end(); eit++) {
                    DWVertexProp dwep;
                    dwep.id = maxBranch.edgeOpMap[*eit].trg;
                    dwep.edgeid = *eit;
                    dwep.weight = maxBranch.edgeOpMap[*eit].weight;
                    Edge edge = make_pair(*rvit2, dwep);
                    EdgeMap::iterator newedge = edgemap.find(edge);
                    DWVertexProp dwep1 = newedge->second.second;
                    int src = newedge->second.first;
                    maxBranch.addEdge(src, dwep1.id, dwep1.weight, dwep1.edgeid);
                }
            }

            // remove vertex
            maxBranch.removeVertex(*rvit2);
        }
    }


    bool DWGraphUtil::checkBranch(DWGraph branch) {
        DWVertexList vl = branch.vertices();
        DWVertexList::iterator vlit;
        DWEdgeList el;
        DWEdgeList::iterator eit;
        for (vlit = vl.begin(); vlit != vl.end(); vlit++) {
            if (branch.in_degree(vlit->first) > 1) {
                el = branch.in_edges(vlit->first);
                cout << "Max Branch Wrong" << endl;
                return false;
            }
        }
        return true;
    }
}
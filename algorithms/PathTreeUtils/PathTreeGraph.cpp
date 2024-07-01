#include "PathTreeGraph.h"

namespace path_tree {
    PathTreeGraph::PathTreeGraph() {
        graph = Path_GRA();
        vl = PathTreeVertexList();
    }

    PathTreeGraph::PathTreeGraph(int size) {
        vsize = size;
        vl = PathTreeVertexList(size);
        graph = Path_GRA(size, PathTree_In_OutList());
    }

    PathTreeGraph::PathTreeGraph(Path_GRA &g, PathTreeVertexList &vlist) {
        vsize = vlist.size();
        graph = g;
        vl = vlist;
    }

    PathTreeGraph::PathTreeGraph(istream &in) {
        readGraph(in);
    }

    PathTreeGraph::~PathTreeGraph() {}

    void PathTreeGraph::printGraph() {
        writeGraph(cout);
    }

    void PathTreeGraph::readGraph(istream &in) {
        string buf;
        getline(in, buf);

        if (buf != "graph_for_greach") {
            cout << "BAD FILE FORMAT!" << endl;
            exit(0);
        }

        int n;
        getline(in, buf);
        istringstream(buf) >> n;
        // initialize
        vsize = n;
        vl = PathTreeVertexList(n);
        graph = Path_GRA(n, PathTree_In_OutList());

        for (int i = 0; i < n; i++)
            addVertex(i);

        string sub;
        int idx;
        int sid = 0;
        int tid = 0;
        while (getline(in, buf)) {
            idx = buf.find(":");
            buf.erase(0, idx + 2);
            while (buf.find(" ") != string::npos) {
                sub = buf.substr(0, buf.find(" "));
                istringstream(sub) >> tid;
                buf.erase(0, buf.find(" ") + 1);
                addEdge(sid, tid);
            }
            ++sid;
        }
    }

    void PathTreeGraph::writeGraph(ostream &out) {
        cout << "PathTreeGraph size = " << graph.size() << endl;
        out << "graph_for_greach" << endl;
        out << vl.size() << endl;

        Path_GRA::iterator git;
        PathTreeEdgeList el;
        PathTreeEdgeList::iterator eit;
        PathTreeVertexList::iterator vit;
        int i = 0;
        for (i = 0; i < vl.size(); i++) {
            out << i << ": ";
            el = graph[i].outList;
            for (eit = el.begin(); eit != el.end(); eit++)
                out << (*eit) << " ";
            out << "#" << endl;
        }
/*
	cout << "** In List for graph **" << endl;
	for (i = 0; i < vl.size(); i++) {
		out << i << ": ";
		el = graph[i].inList;
		for (eit = el.begin(); eit != el.end(); eit++)
			out << (*eit) << " ";
		out << "#" << endl;
	}
*/
    }

    void PathTreeGraph::addVertex(int vid) {
        if (vid >= vl.size()) {
            int size = vl.size();
            for (int i = 0; i < (vid - size + 1); i++) {
                graph.push_back(PathTree_In_OutList());
                vl.push_back(PathTreeVertex());
            }
            vsize = vl.size();
        }

        PathTreeVertex v;
        v.visited = false;
        vl[vid] = v;

        PathTreeEdgeList il = PathTreeEdgeList();
        PathTreeEdgeList ol = PathTreeEdgeList();
        PathTree_In_OutList oil = PathTree_In_OutList();
        oil.inList = il;
        oil.outList = ol;
        graph[vid] = oil;
    }

    void PathTreeGraph::addEdge(int sid, int tid) {
        if (sid >= vl.size())
            addVertex(sid);
        if (tid >= vl.size())
            addVertex(tid);
        // update edge list
        graph[tid].inList.push_back(sid);
        graph[sid].outList.push_back(tid);
    }

    int PathTreeGraph::num_vertices() {
        return vl.size();
    }

    int PathTreeGraph::num_edges() {
        PathTreeEdgeList el;
        Path_GRA::iterator git;
        int num = 0;
        for (git = graph.begin(); git != graph.end(); git++) {
            el = git->outList;
            num += el.size();
        }
        return num;
    }

// return out edges of specified vertex
    PathTreeEdgeList &PathTreeGraph::out_edges(int src) {
        return graph[src].outList;
    }

// return in edges of specified vertex
    PathTreeEdgeList &PathTreeGraph::in_edges(int trg) {
        return graph[trg].inList;
    }

    int PathTreeGraph::out_degree(int src) {
        return graph[src].outList.size();
    }

    int PathTreeGraph::in_degree(int trg) {
        return graph[trg].inList.size();
    }

// get roots of graph (root is zero in_degree vertex)
    vector<int> PathTreeGraph::getRoots() {
        vector<int> roots;
        Path_GRA::iterator git;
        int i = 0;
        for (git = graph.begin(), i = 0; git != graph.end(); git++, i++) {
            if (git->inList.size() == 0)
                roots.push_back(i);
        }

        return roots;
    }

// check whether the edge from src to trg is in the graph
    bool PathTreeGraph::hasEdge(int src, int trg) {
        PathTreeEdgeList el = graph[src].outList;
        PathTreeEdgeList::iterator ei;
        for (ei = el.begin(); ei != el.end(); ei++)
            if ((*ei) == trg)
                return true;
        return false;

    }

// return vertex list of graph
    PathTreeVertexList &PathTreeGraph::vertices() {
        return vl;
    }

    PathTreeGraph &PathTreeGraph::operator=(const PathTreeGraph &g) {
        if (this != &g) {
            graph = g.graph;
            vl = g.vl;
            vsize = g.vsize;
        }
        return *this;
    }

// get a specified vertex property
    PathTreeVertex &PathTreeGraph::operator[](const int vid) {
        return vl[vid];
    }

    PathTreeGraph::PathTreeGraph(hash_map<int, vector<int> > &inlist, hash_map<int, vector<int> > &outlist) {
        vsize = inlist.size();
        vl = PathTreeVertexList(vsize);
        graph = Path_GRA(vsize, PathTree_In_OutList());
        for (int i = 0; i < vsize; i++)
            addVertex(i);
        hash_map<int, vector<int> >::iterator hit, hit1;
        hash_map<int, int> indexmap;
        vector<int> vec;
        vector<int>::iterator vit;
        int k;
        for (hit = inlist.begin(), k = 0; hit != inlist.end(); hit++, k++) {
            indexmap[hit->first] = k;
        }
        for (hit = inlist.begin(), hit1 = outlist.begin(), k = 0; hit != inlist.end(); hit++, hit1++, k++) {
            vec = hit->second;
            for (vit = vec.begin(); vit != vec.end(); vit++)
                graph[k].inList.push_back(indexmap[*vit]);
            vec = hit1->second;
            for (vit = vec.begin(); vit != vec.end(); vit++)
                graph[k].outList.push_back(indexmap[*vit]);
        }
    }

    void PathTreeGraph::extract(hash_map<int, vector<int> > &inlist, hash_map<int, vector<int> > &outlist) {
        for (int i = 0; i < vl.size(); i++) {
            inlist[i] = graph[i].inList;
            outlist[i] = graph[i].outList;
        }
//	printMap(inlist,outlist);
    }

// for test
    void PathTreeGraph::printMap(hash_map<int, vector<int> > &inlist, hash_map<int, vector<int> > &outlist) {
        cout << "==============================================" << endl;
        hash_map<int, vector<int> >::iterator hit;
        vector<int>::iterator vit;
        for (hit = outlist.begin(); hit != outlist.end(); hit++) {
            cout << hit->first << ": ";
            vector<int> vec = hit->second;
            for (vit = vec.begin(); vit != vec.end(); vit++)
                cout << *vit << " ";
            cout << "#" << endl;
        }
        cout << "In List for graph" << endl;
        for (hit = inlist.begin(); hit != inlist.end(); hit++) {
            cout << hit->first << ": ";
            vector<int> vec = hit->second;
            for (vit = vec.begin(); vit != vec.end(); vit++)
                cout << *vit << " ";
            cout << "#" << endl;
        }
        cout << "================================================" << endl;
    }

    PathTreeGraph::PathTreeGraph(const Graph &input_graph) {
        convertGraph(input_graph);
    }

    void PathTreeGraph::convertGraph(const Graph &input_graph) {
        int n = input_graph.size();
        // initialize
        vsize = n;
        vl = PathTreeVertexList(n);
        graph = Path_GRA(n, PathTree_In_OutList());

        for (int i = 0; i < n; i++)
            addVertex(i);

        for (int sid = 0; sid < n; ++sid) {
            for (const int &tid: input_graph.getSuccessors(sid)) {
                addEdge(sid, tid);
            }
        }
    }
}
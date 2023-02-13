/* Copyright (c) Hilmi Yildirim 2010,2011.

The software is provided on an as is basis for research purposes.
There is no additional support offered, nor are the author(s) 
or their institutions liable under any circumstances.
*/
#include "GrailGraph.h"

namespace grail {

    GrailGraph::GrailGraph() {
        graph = Grail_GRA();
        vl = GrailVertexList();
    }

    GrailGraph::GrailGraph(int size) {
        vsize = size;
        vl = GrailVertexList(size);
        graph = Grail_GRA(size, Grail_In_OutList());
    }

    GrailGraph::GrailGraph(Grail_GRA &g, GrailVertexList &vlist) {
        vsize = vlist.size();
        graph = g;
        vl = vlist;
    }

    GrailGraph::GrailGraph(istream &in) {
        readGraph(in);
    }

    GrailGraph::GrailGraph(const Graph &input_graph) {
        convertGraph(input_graph);
    }

    GrailGraph::~GrailGraph() {}

    void GrailGraph::printGraph() {
        writeGraph(cout);
    }

    void GrailGraph::clear() {
        vsize = 0;
        graph.clear();
        vl.clear();
    }

    bool GrailGraph::contains(int src, int trg, int dim) {
        int i;
        for (i = 0; i < dim; i++) {
            if (vl[src].pre->at(i) > vl[trg].pre->at(i))
                return false;
            if (vl[src].post->at(i) < vl[trg].post->at(i))
                return false;
        }
        return true;
    }

    bool GrailGraph::incrementalContains(int src, int trg, int cur) {
        int i;
        for (i = 0; i < cur; i++) {
            if (vl[src].pre->at(i) > vl[trg].pre->at(i))
                return false;
            if (vl[src].post->at(i) < vl[trg].post->at(i))
                return false;
        }
        return true;
    }

    void GrailGraph::strTrimRight(string &str) {
        string whitespaces(" \t");
        int index = str.find_last_not_of(whitespaces);
        if (index != string::npos)
            str.erase(index + 1);
        else
            str.clear();
    }

    void GrailGraph::readGraph(istream &in) {
        string buf;
        getline(in, buf);

        strTrimRight(buf);
        if (buf != "graph_for_greach") {
            cout << "BAD FILE FORMAT!" << endl;
            exit(0);
        }

        int n;
        getline(in, buf);
        istringstream(buf) >> n;
        // initialize
        vsize = n;
        vl = GrailVertexList(n);
        graph = Grail_GRA(n, Grail_In_OutList());

        for (int i = 0; i < n; i++)
            addVertex(i);

        string sub;
        int idx;
        int sid = 0;
        int tid = 0;
        while (getline(in, buf)) {
            strTrimRight(buf);
            idx = buf.find(":");
            buf.erase(0, idx + 2);
            while (buf.find(" ") != string::npos) {
                sub = buf.substr(0, buf.find(" "));
                istringstream(sub) >> tid;
                buf.erase(0, buf.find(" ") + 1);
                if (sid == tid)
                    cout << "Self-edge " << sid << endl;
                if (tid < 0 || tid > n)
                    cout << "Wrong tid " << tid << endl;

                addEdge(sid, tid);
            }
            ++sid;
        }
    }

    void GrailGraph::convertGraph(const Graph &input_graph) {
        int n = input_graph.size();
        // initialize
        vsize = n;
        vl = GrailVertexList(n);
        graph = Grail_GRA(n, Grail_In_OutList());

        for (int i = 0; i < n; i++)
            addVertex(i);

        for (int sid = 0; sid < n; ++sid) {
            for (const int &tid: input_graph.getSuccessors(sid)) {
                addEdge(sid, tid);
            }
        }
    }

    void GrailGraph::writeGraph(ostream &out) {
        cout << "GrailGraph size = " << graph.size() << endl;
        out << "graph_for_greach" << endl;
        out << vl.size() << endl;

        Grail_GRA::iterator git;
        GrailEdgeList el;
        GrailEdgeList::iterator eit;
        GrailVertexList::iterator vit;
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

    void GrailGraph::addVertex(int vid) {
        if (vid >= vl.size()) {
            int size = vl.size();
            for (int i = 0; i < (vid - size + 1); i++) {
                graph.push_back(Grail_In_OutList());
                vl.push_back(GrailVertex(vid + i));
            }
            vsize = vl.size();
        }

        GrailVertex v;
        v.id = vid;
        v.top_level = -1;
        v.visited = false;
        vl[vid] = v;

        GrailEdgeList il = GrailEdgeList();
        GrailEdgeList ol = GrailEdgeList();
        Grail_In_OutList oil = Grail_In_OutList();
        oil.inList = il;
        oil.outList = ol;
        graph[vid] = oil;
    }

    void GrailGraph::addEdge(int sid, int tid) {
        if (sid >= vl.size())
            addVertex(sid);
        if (tid >= vl.size())
            addVertex(tid);
        // update edge list
        graph[tid].inList.push_back(sid);
        graph[sid].outList.push_back(tid);
    }

    int GrailGraph::num_vertices() {
        return vl.size();
    }

    int GrailGraph::num_edges() {
        GrailEdgeList el;
        Grail_GRA::iterator git;
        int num = 0;
        for (git = graph.begin(); git != graph.end(); git++) {
            el = git->outList;
            num += el.size();
        }
        return num;
    }

// return out edges of specified vertex
    GrailEdgeList &GrailGraph::out_edges(int src) {
        return graph[src].outList;
    }

// return in edges of specified vertex
    GrailEdgeList &GrailGraph::in_edges(int trg) {
        return graph[trg].inList;
    }

    int GrailGraph::out_degree(int src) {
        return graph[src].outList.size();
    }

    int GrailGraph::in_degree(int trg) {
        return graph[trg].inList.size();
    }

// get roots of graph (root is zero in_degree vertex)
    vector<int> GrailGraph::getRoots() {
        vector<int> roots;
        Grail_GRA::iterator git;
        int i = 0;
        for (git = graph.begin(), i = 0; git != graph.end(); git++, i++) {
            if (git->inList.size() == 0)
                roots.push_back(i);
        }

        return roots;
    }

// check whether the edge from src to trg is in the graph
    bool GrailGraph::hasEdge(int src, int trg) {
        GrailEdgeList el = graph[src].outList;
        GrailEdgeList::iterator ei;
        for (ei = el.begin(); ei != el.end(); ei++)
            if ((*ei) == trg)
                return true;
        return false;

    }

// return vertex list of graph
    GrailVertexList &GrailGraph::vertices() {
        return vl;
    }

    GrailGraph &GrailGraph::operator=(const GrailGraph &g) {
        if (this != &g) {
            graph = g.graph;
            vl = g.vl;
            vsize = g.vsize;
        }
        return *this;
    }

// get a specified vertex property
    GrailVertex &GrailGraph::operator[](const int vid) {
        return vl[vid];
    }

    const double GrailGraph::actualgap(const int vid) {
        return vl[vid].mingap;
//	return vl[vid].mingap - vl[vid].tcs;
    }

    const double GrailGraph::tcs(const int vid) {
        return vl[vid].tcs;
    }

    GrailGraph::GrailGraph(hash_map<int, vector<int> > &inlist, hash_map<int, vector<int> > &outlist) {
        vsize = inlist.size();
        vl = GrailVertexList(vsize);
        graph = Grail_GRA(vsize, Grail_In_OutList());
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

    void GrailGraph::extract(hash_map<int, vector<int> > &inlist, hash_map<int, vector<int> > &outlist) {
        for (int i = 0; i < vl.size(); i++) {
            inlist[i] = graph[i].inList;
            outlist[i] = graph[i].outList;
        }
//	printMap(inlist,outlist);
    }

// for test
    void GrailGraph::printMap(hash_map<int, vector<int> > &inlist, hash_map<int, vector<int> > &outlist) {
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

    long long GrailGraph::indexSize() const {
        long long size = vsize * sizeof(Grail_GRA);
        for (auto in_out : graph) {
            size += in_out.inList.size() * sizeof(int);
            size += in_out.outList.size() * sizeof(int);
        }
        for (auto veretx : vl) {
            size += veretx.pre->size() * sizeof(int);
            size += veretx.post->size() * sizeof(int);
            size += veretx.middle->size() * sizeof(int);
        } 
        return size;
    }
}
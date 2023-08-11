#ifndef _GRAPH_FILE_H_#define _GRAPH_FILE_H_#include <iostream>#include <fstream>#include "../Graph.h"namespace butr {    using namespace ::std;#define SPACESIZE 20#define QUERYNUM 50#define QUERY//#define QUERYINFO//#define DFSQUERY#ifdef QUERYINFO    int evF;    int dimF;    int rootT;    int hopT;    int hopF;    int allReach;#endif    struct EDGE {        int start;        int degree;    };    EDGE *outEdge, *inEdge;    int *edge, *edgeR;    int vertexNum, edgeNum;    int *q, *indice, *visited;    int *zeroInDegree, *zeroOutDegree;    int stackTop;    int reachable, unreachable;    void initGraph();    void readGraph(FILE *);    void reverse();    void zeroVertex();    void destroyGraph();    void outGraph();//////////////////////////////////////////////////////////////////////////    void initGraph() {        outEdge = new EDGE[vertexNum];        inEdge = new EDGE[vertexNum];        edge = new int[edgeNum];        edgeR = new int[edgeNum];        q = new int[vertexNum];        indice = new int[vertexNum];        visited = new int[vertexNum];        zeroInDegree = new int[vertexNum + 1];        zeroOutDegree = new int[vertexNum + 1];    }    void readGraph(FILE *inFile) {        int ret = 0;        ret = fscanf(inFile, "%d", &vertexNum);        ret = fscanf(inFile, "%d", &edgeNum);        initGraph();        for (int i = 0; i < vertexNum; ++i) {            inEdge[i].degree = 0;        }        int m = 0, sid, degree, tid;        while ((ret = fscanf(inFile, "%d", &sid)) != EOF) {            ret = fscanf(inFile, "%d", &degree);            outEdge[sid].start = m;            outEdge[sid].degree = degree;            for (int i = 0; i < degree; ++i) {                ret = fscanf(inFile, "%d", &tid);                inEdge[tid].degree++;                edge[m++] = tid;            }        }        reverse();    }    void convertGraph(const Graph &graph) {        vertexNum = graph.size();        edgeNum = 0;        for (int i = 0; i < vertexNum; ++i) {            edgeNum += graph.getOutDegree(i);        }        initGraph();        for (int i = 0; i < vertexNum; ++i) {            inEdge[i].degree = 0;        }        int m = 0, sid, degree;        for (sid = 0; sid < graph.size(); ++sid) {            outEdge[sid].start = m;            outEdge[sid].degree = graph.getOutDegree(sid);            for (const auto &tid : graph.getSuccessors(sid)) {                inEdge[tid].degree++;                edge[m++] = tid;            }        }        reverse();    }    void reverse() {        int sum = 0, i;        for (i = 0; i < vertexNum; ++i) {            inEdge[i].start = sum;            sum += inEdge[i].degree;        }        int endPOI, curEdge, e;        for (i = 0; i < vertexNum; ++i) {            e = outEdge[i].start;            endPOI = e + outEdge[i].degree;            for (; e < endPOI; ++e) {                curEdge = edge[e];                edgeR[inEdge[curEdge].start++] = i;            }        }        sum = 0;        for (i = 0; i < vertexNum; ++i) {            inEdge[i].start = sum;            sum += inEdge[i].degree;        }    }    void zeroVertex() {        int zeroIn = 0;        int zeroOut = 0;        for (int i = 0; i < vertexNum; ++i) {            if (0 == outEdge[i].degree) {                if (0 != inEdge[i].degree) {                    zeroOutDegree[++zeroOut] = i;                } else {                    zeroOutDegree[++zeroOut] = i;                    zeroInDegree[++zeroIn] = i;                }            } else if (0 == inEdge[i].degree) {                zeroInDegree[++zeroIn] = i;            }        }        zeroInDegree[0] = zeroIn;        zeroOutDegree[0] = zeroOut;    }    void destroyGraph() {        delete[]outEdge;        delete[]inEdge;        delete[]edge;        delete[]edgeR;        delete[]q;        delete[]indice;        delete[]visited;        delete[]zeroInDegree;        delete[]zeroOutDegree;    }    void outGraph() {        int start, end;        for (int i = 0; i < vertexNum; ++i) {            start = outEdge[i].start;            end = start + outEdge[i].degree;            while (start < end) {                cout << endl << i << "->" << edge[start++];            }        }        cout << endl << "reverse graph" << endl;        for (int i = 0; i < vertexNum; ++i) {            cout << endl << i;            start = inEdge[i].start;            end = start + inEdge[i].degree;            while (start < end) {                cout << " " << edgeR[start++];            }        }    }}#endif
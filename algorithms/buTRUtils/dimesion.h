#ifndef _DIMENSION_FILE_H_

#define _DIMENSION_FILE_H_

//#include "priorityQueue.h"

#include "graph.h"

#include "equalVertex.h"

#include "hopLabel.h"

#include <iostream>

namespace butr {

    using namespace ::std;


    extern int *indice, *q;

    extern Root *R;

//////////////////////////////////////////////////////////////////////////





    struct Dimension {

        int first, second;

        int firstend, secondend;

        int firstR, secondR;

        int firstRend, secondRend;

    };

    int firDimS, secDimS;

    int firDimSR, secDimSR;

    int firDimSend, secDimSend;

    int firDimSRend, secDimSRend;

    int firDimT, secDimT;

    int firDimTR, secDimTR;

    int firDimTend, secDimTend;

    int firDimTRend, secDimTRend;

    int sVidUp, sVidDown, tVidUp, tVidDown;

    Dimension *Di;

    int **idx;

    int ***local;

    bool *flag;

    int dfsIterm;

    int all;

    struct indexNode {

        int upRoot;

        int downRoot;

        int downup;

        int first, second;

//	int firstend;

        int firstR, secondR;

//	int firstRend;

    };

    indexNode *idxN;



//////////////////////////////////////////////////////////////////////////



    void initDim();

    void createDimLabel();

    void createDimLabelImp();

    void createDimLabelR();

    void createDimLabelRImp();

    void destroyDimLabel();

    void outDim();


    void initDim() {

        idx = new int *[vertexNum];

        flag = new bool[vertexNum];

        for (int i = 0; i < vertexNum; i++) {

            flag[i] = false;

        }

        Di = new Dimension[vertexNum];

        idxN = new indexNode[vertexNum];

#ifdef QUERYINFO

        evF = 0;

        dimF = 0;

        rootT = 0;

        hopT = 0;

        hopF = 0;

#endif


        reachable = 0;

        unreachable = 0;

        all = 0;

    }


    void createDimLabelLeftToRight() {

        for (int i = 0; i < vertexNum; ++i) {

            virInDegree[i] = inEdge[i].degree;

        }

        int curV, end, start, vid;

        int topologyNum = 0;

        stackTop = -1;

        int zeroInNum = zeroInDegree[0];

        for (int i = 1; i <= zeroInNum; ++i) {

            curV = zeroInDegree[i];

            stackTree[++stackTop].vertexID = curV;

            stackTree[stackTop].mark = -1;

            while (stackTop >= 0) {

                if (0 == stackTree[stackTop].mark) {

                    curV = stackTree[stackTop].vertexID;

                    Di[curV].firstend = topologyNum - 1;

                    --stackTop;

                } else {

                    curV = stackTree[stackTop].vertexID;

                    stackTree[stackTop].mark = 0;

                    Di[curV].first = topologyNum++;


                    start = outEdge[curV].start;

                    end = start + outEdge[curV].degree;

                    while (start < end) {

                        vid = edge[start++];

                        --virInDegree[vid];

                        if (0 == virInDegree[vid]) {

                            stackTree[++stackTop].vertexID = vid;

                            stackTree[stackTop].mark = -1;

                        }

                    }


                    outEdge[curV].degree = 0;

                    //sort adject

                    start = inEdge[curV].start;

                    end = inEdge[curV].degree + start;

                    while (start < end) {

                        vid = edgeR[start++];

                        int ttt = outEdge[vid].start + outEdge[vid].degree;

                        outEdge[vid].degree++;

                        edge[ttt] = curV;

                    }

                }

            }

        }

    }


    void createDimLabelRightToLeft() {

        for (int i = 0; i < vertexNum; ++i) {

            virInDegree[i] = inEdge[i].degree;

        }

        int curV, end, start, vid;

        int topologyNum = 0;

        int zeroInNum = zeroInDegree[0];

        stackTop = -1;

        for (int i = zeroInNum; i > 0; --i) {

            curV = zeroInDegree[i];

            stackTree[++stackTop].vertexID = curV;

            stackTree[stackTop].mark = -1;


            while (stackTop >= 0) {

                if (0 == stackTree[stackTop].mark) {

                    curV = stackTree[stackTop].vertexID;

                    Di[curV].secondend = topologyNum - 1;

                    --stackTop;

                } else {

                    curV = stackTree[stackTop].vertexID;

                    stackTree[stackTop].mark = 0;


                    Di[curV].second = topologyNum++;


                    start = outEdge[curV].start;

                    end = start + outEdge[curV].degree;

                    while (start < end) {

                        //vid = edge[--end];

                        vid = edge[start++];

                        --virInDegree[vid];

                        if (0 == virInDegree[vid]) {

                            stackTree[++stackTop].vertexID = vid;

                            stackTree[stackTop].mark = -1;

                        }

                    }

                }

            }

        }

    }


    void createDimLabelRLeftToRight() {

        int *virOutDegree = virInDegree;

        for (int i = 0; i < vertexNum; ++i) {

            virOutDegree[i] = outEdge[i].degree;

        }

        int curV, end, start, vid;

        int topologyNum = 0;

        int zeroOutNum = zeroOutDegree[0];

        stackTop = -1;

        for (int i = 1; i <= zeroOutNum; ++i) {

            curV = zeroOutDegree[i];

            stackTree[++stackTop].vertexID = curV;

            stackTree[stackTop].mark = -1;


            while (stackTop >= 0) {

                if (0 == stackTree[stackTop].mark) {

                    curV = stackTree[stackTop].vertexID;

                    Di[curV].firstRend = topologyNum - 1;

                    --stackTop;

                } else {

                    curV = stackTree[stackTop].vertexID;

                    stackTree[stackTop].mark = 0;


                    Di[curV].firstR = topologyNum++;


                    start = inEdge[curV].start;

                    end = start + inEdge[curV].degree;

                    while (start < end) {

                        vid = edgeR[start++];

                        --virOutDegree[vid];

                        if (0 == virOutDegree[vid]) {

                            stackTree[++stackTop].vertexID = vid;

                            stackTree[stackTop].mark = -1;

                        }

                    }

                    inEdge[curV].degree = 0;

                    //sort adject

                    start = outEdge[curV].start;

                    end = outEdge[curV].degree + start;

                    while (start < end) {

                        vid = edge[start++];

                        int ttt = inEdge[vid].start + inEdge[vid].degree;

                        inEdge[vid].degree++;

                        edgeR[ttt] = curV;

                    }

                }

            }

        }

    }


    void createDimLabelRRightToLeft() {

        int *virOutDegree = virInDegree;

        for (int i = 0; i < vertexNum; ++i) {

            virOutDegree[i] = outEdge[i].degree;

        }

        int curV, end, start, vid;

        int topologyNum = 0;

        int zeroOutNum = zeroOutDegree[0];

        stackTop = -1;

        for (int i = zeroOutNum; i > 0; --i) {

            curV = zeroOutDegree[i];

            stackTree[++stackTop].vertexID = curV;

            stackTree[stackTop].mark = -1;


            while (stackTop >= 0) {

                if (0 == stackTree[stackTop].mark) {

                    curV = stackTree[stackTop].vertexID;

                    Di[curV].secondRend = topologyNum - 1;

                    --stackTop;

                } else {

                    curV = stackTree[stackTop].vertexID;

                    stackTree[stackTop].mark = 0;


                    Di[curV].secondR = topologyNum++;


                    start = inEdge[curV].start;

                    end = start + inEdge[curV].degree;

                    while (start < end) {

                        //vid = edgeR[--end];

                        vid = edgeR[start++];

                        --virOutDegree[vid];

                        if (0 == virOutDegree[vid]) {

                            stackTree[++stackTop].vertexID = vid;

                            stackTree[stackTop].mark = -1;

                        }

                    }

                }

            }

        }

    }


    void destroyDimLabel() {

        delete[]Di;

    }

    void sortIndex() {

        int j, k;

        for (int i = 0; i < vertexNum; i++) {

            int si = convertexEqualVertex[i];

            if (flag[si] == false) {

                flag[si] = true;

                idxN[si].first = Di[si].first;

                idxN[si].second = Di[si].second;

                idxN[si].firstR = Di[si].firstR;

                idxN[si].secondR = Di[si].secondR;

                idxN[si].upRoot = R[si].ur;

                idxN[si].downRoot = R[si].dr;

                idxN[si].downup = R[R[si].dr].ur;

            }

        }

    }


    bool queryImpImp(const int &sid, const int &tid) {
        // xJump modify rule
        if (sid == tid) {
            reachable++;
            return true;
        }

        int sVid = convertexEqualVertex[sid];

        int tVid = convertexEqualVertex[tid];

        if (sVid == tVid) {

            unreachable++;

#ifdef QUERYINFO

            evF++;

#endif

            return false;

        }

        int sdown = idxN[sVid].downRoot, tup = idxN[tVid].upRoot;

        if (sdown == tup) {

            reachable++;

            return true;

        }


        if (idxN[sVid].first < idxN[tVid].first && idxN[sVid].second < idxN[tVid].second &&
            idxN[sVid].firstR > idxN[tVid].firstR && idxN[sVid].secondR > idxN[tVid].secondR) {

            int sup = idxN[sVid].upRoot, sdownup = idxN[sVid].downup;

            int tdown = idxN[tVid].downRoot;

            if (sup == tup || sdown == tdown || sdownup == tup) {

#ifdef QUERYINFO

                rootT++;

#endif

                reachable++;

                return true;

            }

#ifdef QUERYINFO

            if (interQuery(sVidDown, tVidUp))

            {

                hopT++;

                return true;

            }

            else

            {

                hopF++;

                return false;

            }

#else

            if (interQuery(sdown, tup)) {

                reachable++;

                return true;

            } else {

                unreachable++;

                return false;

            }

#endif

        } else {

#ifdef QUERYINFO

            dimF++;

#endif

            unreachable++;

            return false;

        }

    }


    bool DFSRchR(const int sid, const int tid) {

        if (sid == tid)

            return true;

        int sVid = convertexEqualVertex[sid];

        int tVid = convertexEqualVertex[tid];

        if (sVid == tVid) {

#ifdef QUERYINFO

            evF++;

#endif

            return false;

        }


        int firDimS = Di[sVid].first, secDimS = Di[sVid].second;

        int firDimSR = Di[sVid].firstR, secDimSR = Di[sVid].secondR;

        int sVidUp, sVidDown;


        int firDimT = Di[tVid].first, secDimT = Di[tVid].second;

        int firDimTR = Di[tVid].firstR, secDimTR = Di[tVid].secondR;

        int tVidUp, tVidDown;


        if (firDimS < firDimT && secDimS < secDimT && firDimSR > firDimTR && secDimSR > secDimTR) {

            sVidUp = R[sVid].ur;

            sVidDown = R[sVid].dr;

            tVidUp = R[tVid].ur;

            tVidDown = R[tVid].dr;


            if (sVidUp == tVidUp || sVidDown == tVidDown) {

#ifdef QUERYINFO

                rootT++;

#endif

                return true;

            }

            if (sVidDown == tVidUp || R[sVidDown].ur == tVidUp) {

#ifdef QUERYINFO

                rootT++;

#endif

                return true;

            }

            int start, end, vid;

            start = inEdge[tVidUp].start;

            end = start + inEdge[tVidUp].degree;

            while (start < end) {

                vid = edgeR[start++];

                if (dfsIterm != visited[vid]) {

                    visited[vid] = dfsIterm;

                    if (DFSRchR(sVidDown, vid))

                        return true;

                }

            }

            return false;

        } else {

#ifdef QUERYINFO

            dimF++;

#endif

            return false;

        }

        return false;

    }


    bool OSRchR(const int &sid, const int &tid) {

        int sVid = convertexEqualVertex[sid];

        int tVid = convertexEqualVertex[tid];

        if (sVid == tVid) {

#ifdef QUERYINFO

            evF++;

#endif

            unreachable++;

            return false;

        }

        firDimS = Di[sVid].first, secDimS = Di[sVid].second;

        firDimSR = Di[sVid].firstR, secDimSR = Di[sVid].secondR;


        firDimT = Di[tVid].first, secDimT = Di[tVid].second;

        firDimTR = Di[tVid].firstR, secDimTR = Di[tVid].secondR;


        if (firDimS < firDimT && secDimS < secDimT && firDimSR > firDimTR && secDimSR > secDimTR) {

            sVidUp = R[sVid].ur;

            sVidDown = R[sVid].dr;

            tVidUp = R[tVid].ur;

            tVidDown = R[tVid].dr;


            if (sVidUp == tVidUp || sVidDown == tVidDown) {

#ifdef QUERYINFO

                rootT++;

#endif

                reachable++;

                return true;

            }

            if (sVidDown == tVidUp || R[sVidDown].ur == tVidUp) {

#ifdef QUERYINFO

                rootT++;

#endif

                reachable++;

                return true;

            }

            int start, end, vid;

            start = inEdge[tVidUp].start;

            end = start + inEdge[tVidUp].degree;

            while (start < end) {

                vid = edgeR[start++];

                if (dfsIterm != visited[vid]) {

                    visited[vid] = dfsIterm;

                    if (DFSRchR(sVidDown, vid))

                        return true;

                }

            }

            return false;

        } else {

#ifdef QUERYINFO

            dimF++;

#endif

            unreachable++;

            return false;

        }

    }


    bool DFSRch(const int sid, const int tid) {

        if (sid == tid)

            return true;

        int sVid = convertexEqualVertex[sid];

        int tVid = convertexEqualVertex[tid];

        if (sVid == tVid) {

#ifdef QUERYINFO

            evF++;

#endif

            return false;

        }


        int firDimS = Di[sVid].first, secDimS = Di[sVid].second;

        int firDimSR = Di[sVid].firstR, secDimSR = Di[sVid].secondR;

        int sVidUp, sVidDown;


        int firDimT = Di[tVid].first, secDimT = Di[tVid].second;

        int firDimTR = Di[tVid].firstR, secDimTR = Di[tVid].secondR;

        int tVidUp, tVidDown;


        if (firDimS < firDimT && secDimS < secDimT && firDimSR > firDimTR && secDimSR > secDimTR) {

            sVidUp = R[sVid].ur;

            sVidDown = R[sVid].dr;

            tVidUp = R[tVid].ur;

            tVidDown = R[tVid].dr;


            if (sVidUp == tVidUp || sVidDown == tVidDown) {

#ifdef QUERYINFO

                rootT++;

#endif

                return true;

            }

            if (sVidDown == tVidUp || R[sVidDown].ur == tVidUp) {

#ifdef QUERYINFO

                rootT++;

#endif

                return true;

            }

            int start, end, vid;

            start = outEdge[sVidDown].start;

            end = start + outEdge[sVidDown].degree;

            while (start < end) {

                vid = edge[start++];

                if (dfsIterm != visited[vid]) {

                    visited[vid] = dfsIterm;

                    if (DFSRch(vid, tVidUp))

                        return true;

                }

            }

            return false;

        } else {

#ifdef QUERYINFO

            dimF++;

#endif

            return false;

        }

        return false;

    }


    bool OSRch(const int &sid, const int &tid) {

        int sVid = convertexEqualVertex[sid];

        int tVid = convertexEqualVertex[tid];

        if (sVid == tVid) {

#ifdef QUERYINFO

            evF++;

#endif

            unreachable++;

            return false;

        }

        firDimS = Di[sVid].first, secDimS = Di[sVid].second;

        firDimSR = Di[sVid].firstR, secDimSR = Di[sVid].secondR;


        firDimT = Di[tVid].first, secDimT = Di[tVid].second;

        firDimTR = Di[tVid].firstR, secDimTR = Di[tVid].secondR;


        if (firDimS < firDimT && secDimS < secDimT && firDimSR > firDimTR && secDimSR > secDimTR) {

            sVidUp = R[sVid].ur;

            sVidDown = R[sVid].dr;

            tVidUp = R[tVid].ur;

            tVidDown = R[tVid].dr;


            if (sVidUp == tVidUp || sVidDown == tVidDown) {

#ifdef QUERYINFO

                rootT++;

#endif

                reachable++;

                return true;

            }

            if (sVidDown == tVidUp || R[sVidDown].ur == tVidUp) {

#ifdef QUERYINFO

                rootT++;

#endif

                reachable++;

                return true;

            }

            int start, end, vid;

            start = outEdge[sVidDown].start;

            end = start + outEdge[sVidDown].degree;

            while (start < end) {

                vid = edge[start++];

                if (dfsIterm != visited[vid]) {

                    visited[vid] = dfsIterm;

                    if (DFSRch(vid, tVidUp))

                        return true;

                }

            }

            return false;

        } else {

#ifdef QUERYINFO

            dimF++;

#endif

            unreachable++;

            return false;

        }

    }


    void outDim() {

        cout << endl << "dim" << endl;

        for (int i = 0; i < vertexNum; ++i) {

            cout << i << ": " << Di[i].first << "-" << Di[i].firstend << " " << Di[i].second <<

                 "-" << Di[i].secondend << endl;

        }

        for (int i = 0; i < vertexNum; ++i) {

            cout << i << ": " << Di[i].firstR << "-" << Di[i].firstRend << " " << Di[i].secondR <<

                 "-" << Di[i].secondRend << endl;

        }

        cout << endl;

    }

}

#endif

#include "buTRWrapper.h"

#include "buTRUtils/graph.h"
#include "buTRUtils/transitiveReduction.h"
#include "buTRUtils/butr.h"
#include "buTRUtils/equalVertex.h"
#include "buTRUtils/hopLabel.h"
#include "buTRUtils/dimesion.h"

namespace butr {
    buTRWrapper::buTRWrapper() {

    }

    string buTRWrapper::getName() const {
        return "buTR";
    }

    string buTRWrapper::getParams() const {
        return {};
    }

    void buTRWrapper::construction(const Graph &graph) {
        convertGraph(graph);

        //data init
        zeroVertex();
        initPMTTR();

        //PMTTR
        constructTree();
        constructTreeR();
        initURN();
        initBU();
        estimationOutG();
        sortNewGraph();
        getPoTree();
        buTree();
        fun();

        //equal vertex
        initEqVertex();
        equalNum();
        delEqualVertex();

        //P2H
        initHopLabel();
        initUpRoot();
        initDownRoot();

        portalVertexOrder();
        createP2HLabel();

        //dimension labe
        initDim();
        createDimLabelLeftToRight();
        createDimLabelRightToLeft();
        createDimLabelRLeftToRight();
        createDimLabelRRightToLeft();
//        outIndexSize();
        sortIndex();
    }

    bool buTRWrapper::TC_haspath(int source, int target) {
        return queryImpImp(source, target);
    }

    void buTRWrapper::reset() {
        destroyPMTTR();
        destroyBU();
        destroyEqVertex();
        destroyHopLabel();
        destroyDimLabel();
        destroyGraph();
    }

    unsigned long long buTRWrapper::getIndexSize() const {
        int count = 0;

        int size = 0;

        for (int i = 0; i < vertexNum; ++i) {

            if (convertexEqualVertex[i] != i) {

                count++;

            }

            size += outLabel[i].size();

            size += inLabel[i].size();

        }

//        double indexSize=((vertexNum-count)*7+vertexNum+size)*4.0/(1024*1024);
        return ((vertexNum - count) * 7 + vertexNum + size) * sizeof(int);
    }
} // butr
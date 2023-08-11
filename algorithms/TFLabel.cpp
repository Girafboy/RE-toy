#include "TFLabel.h"

#include "TfLabelUtils/Util.h"

namespace tf_label {
    TFLabel::TFLabel() = default;

    std::string TFLabel::getName() const {
        return "TF-label";
    }

    std::string TFLabel::getParams() const {
        return std::string();
    }

    void TFLabel::reset() {
        if (SL != nullptr) {
            delete[] SL;
            SL = nullptr;
        }
        if (dag != nullptr) {
            free(dag);
            dag = nullptr;
        }
        if (topo != nullptr) {
            free(topo);
            topo = nullptr;
        }
        vertexN = 0;
        edgeN = 0;
    }

    unsigned long long TFLabel::getIndexSize() const {
        size_t SL_size = 0;
        for (int i = 0; i < 2 * vertexN; ++i) {
            SL_size += SL[i].size();
        }
        return (SL_size + 2 * vertexN) * sizeof(int);
    }

    void TFLabel::construction(const Graph &graph) {
        // load graph
        vertexN = graph.size();
        edgeN = graph.number_of_edges();
        int m = 0;
        creatM(dag);
//        memset(ind, 0, sizeof(int) * vertexN);
        start[0] = 0;
        for (int u = 0; u < vertexN; ++u) {
            int deg = graph.getOutDegree(u);
            start[u] = m;
            outd[u] = deg;
            ind[u] = graph.getInDegree(u);
            auto successors = graph.getSuccessors(u);
            for (const auto &v : successors) {
                edge[m++] = v;
            }
        }
        start[vertexN] = m;
        for (int i = vertexN - 1; i > 0; --i)  //i>u?
            if ((outd[i] == 0) & (start[i] == 0))  //change
                start[i] = start[i + 1];

        // generate index
        findHD();
        Hdfs();
        DAG(dag);
        topoSort_B(topo);
        removeHD();
        char tmpf[5] = "g";
        for (char *i = tmpf; *i != '\0'; i++){
            s[tgraph++] = *i;
        }
        pt=tgraph;
        TFHierachy();
        delet();
        transform();
        labelConstruction(SL);
        TLabel(SL);

        // recover
        for (int i = 0; i < max_round; ++i) {
            SLin[i].clear();
            SLout[i].clear();
            deleV[i].clear();
            startSLout[i].clear();
            startSLin[i].clear();
            inH.clear();
            outH.clear();
        }
        level = 0;
        rround = 0;
        tgraph = 0;
        pt = 0;
        while (!dS.empty()) {
            dS.pop();
        }
    }

    bool TFLabel::TC_haspath(int source, int target) {
        if (source == target) {
            return true;
        }
        int sccN = vertexN;
        int p, q, i2, j2, size1, size2;
        vector<int> intArray1, intArray2;
        p = source;
        q = target;
        if (dag[p] != dag[q]){    //after callapsing into super node, in the same connected DAG?
            return false;
        }
        else {
            if (topo[p] >= topo[q]){      //topo means the topo order
                return false;
            }
            else {
                i2 = j2 = 0;

                //SL should int** type
                intArray1=SL[p];
                intArray2=SL[q + sccN];

                //SLsize[i] is the size of SL[i]
                size1 = intArray1.size();
                size2 = intArray2.size();
                while ((i2 < size1) && (j2 < size2)) {
                    //SL[0~vertexN-1] store the out label of each vertex, SL[vertexN~2*vertexN-1] store the in label of each vertex

                    if(intArray1[i2] < intArray2[j2]){
                        ++i2;
                    }
                    else if(intArray1[i2] > intArray2[j2]){
                        ++j2;
                    }
                    else{//		if (intArray1[i2] == intArray2[j2])
                        return true;
                        break;
                    }
                }

                if((i2 == size1) || (j2 == size2)){
                    return false;
                }
            }
        }
        return false;
    }
} // tf_label
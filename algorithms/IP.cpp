#include "IP.h"

namespace ip {
    extern int* flag;
    extern int querycnt;

    IP::IP(int k, int h, int mu) : k(k), h(h), mu(mu) {
        g.KEYNUM = k;
        g.HUGENODENUM = h;
        g.hugenode = mu;
    }

    std::string IP::getName() const {
        return "IP";
    }

    std::string IP::getParams() const {
        return "k = " + std::to_string(k) + " h = " + std::to_string(h) + " mu = " + std::to_string(mu);
    }

    void IP::construction(const Graph &graph) {
        g.convertGraph(graph);
        g.ComputeLayerLabel();
        g.ComputeHugeNodeList();
        g.InitialProLabel();
        g.ComputeProLabelIn();
        g.ComputeProLabelOut();
        g.reachindex.resize(g.vsize);
        for (int u = 0; u < g.vsize; u++) {
            g.reachindex[u].OutLimit = g.ProLabelOut[u].size();
            g.reachindex[u].InLimit = g.ProLabelIn[u].size() + g.reachindex[u].OutLimit;
            g.reachindex[u].HLimit = g.HugeNodeList[u].size() + g.reachindex[u].InLimit;
            g.reachindex[u].Label = new int[g.reachindex[u].HLimit];
            std::copy(g.ProLabelOut[u].begin(), g.ProLabelOut[u].end(), g.reachindex[u].Label);
            std::copy(g.ProLabelIn[u].begin(), g.ProLabelIn[u].end(), g.reachindex[u].Label + g.reachindex[u].OutLimit);
            std::copy(g.HugeNodeList[u].begin(), g.HugeNodeList[u].end(), g.reachindex[u].Label + g.reachindex[u].InLimit);
            g.reachindex[u].layerup = g.layerup[u];
            g.reachindex[u].layerdown = g.layerdown[u];
        }
        querycnt = 0;
        flag = new int[g.vsize];
        memset(flag, 0, sizeof(int) * g.vsize);
    }

    void IP::reset() {
        g.clear();
        g.reachindex.clear();
        g.ProLabelOut.clear();
        g.ProLabelIn.clear();
        g.layerup.clear();
        g.layerdown.clear();
        querycnt = 0;
        delete[] flag;
    }

    bool IP::TC_haspath(int source, int target) {
        if (source == target)
            return true;
        else if (g.reachindex[source].layerdown >= g.reachindex[target].layerdown || g.reachindex[source].layerup <= g.reachindex[target].layerup)
            return false;
        else {
            querycnt++;
            return g.ProLabelQueryDFS2(source, target);
        }
    }

    unsigned long long IP::getIndexSize() const {
        unsigned long long count1 = 0;
        if (g.HUGENODENUM != 0) {
            for (int i = 0; i < g.vsize; i++){
                count1 += g.HugeNodeList[i].size();
            }
        }
        unsigned long long count2 = 0;
        for (int i = 0; i < g.vsize; i++){
            count2 += g.ProLabelOut[i].size();
            count2 += g.ProLabelIn[i].size();
        }
        return 4 * (count1 + count2 + g.vsize);
    }
} // ip
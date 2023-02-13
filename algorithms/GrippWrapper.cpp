//
// Created by 王星力 on 2022/12/27.
//

#include "GrippWrapper.h"

#include "GrippUtils/Gripp.h"
#include "GrippUtils/GrippGraph.h"

namespace gripp {
    GrippWrapper::GrippWrapper() {

    }

    std::string GrippWrapper::getName() const {
        return "GRIPP";
    }

    std::string GrippWrapper::getParams() const {
        return std::string();
    }

    void GrippWrapper::construction(const Graph &graph) {
        GrippGraph * gp = new GrippGraph(Util::MAX_NODE);
        gp->convert(graph);
        grp = new Gripp(Util::MAX_NODE);
        grp->LoadGraph(gp);
        delete gp;
    }

    void GrippWrapper::reset() {
        delete grp;
    }

    bool GrippWrapper::TC_haspath(int source, int target) {
//        std::cout << (unsigned)source << (unsigned)target << std::endl;
        return grp->CanReach((unsigned)source, (unsigned)target);
    }

    long long GrippWrapper::getIndexSize() const {
        return grp->getIndexSize();
    }
} // gripp
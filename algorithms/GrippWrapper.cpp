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
        return {};
    }

    void GrippWrapper::construction(const Graph &graph) {
        GrippGraph *gp = new GrippGraph(Util::MAX_NODE);
        gp->convert(graph);
        grp = new Gripp(Util::MAX_NODE);
        grp->LoadGraph(gp);
        delete gp;
    }

    void GrippWrapper::reset() {
        delete grp;
    }

    bool GrippWrapper::TC_haspath(int source, int target) {
        return grp->CanReach((unsigned) source, (unsigned) target);
    }

    unsigned long long GrippWrapper::getIndexSize() const {
        return grp->getIndexSize();
    }
} // gripp
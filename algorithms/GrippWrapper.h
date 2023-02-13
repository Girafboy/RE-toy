//
// Created by 王星力 on 2022/12/27.
//

#ifndef XJUMP_GRIPPWRAPPER_H
#define XJUMP_GRIPPWRAPPER_H

#include "Algorithm.h"
#include "../Graph.h"
#include "GrippUtils/Gripp.h"
#include "GrippUtils/GrippGraph.h"

namespace gripp {
// todo: 用Util::MAX_NODE的地方都可以优化
    class GrippWrapper : public Algorithm {
    private:
        Gripp * grp;
    public:
        GrippWrapper();

        std::string getName() const override;

        std::string getParams() const override;

        void reset() override;

        void construction(const Graph &graph) override;

        bool TC_haspath(int source, int target) override;

        long long getIndexSize() const override;
    };

} // gripp

#endif //XJUMP_GRIPPWRAPPER_H

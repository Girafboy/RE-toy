//
// Created by 王星力 on 2022/12/4.
//

#ifndef XJUMP_PATHTREEWRAPPER_H
#define XJUMP_PATHTREEWRAPPER_H


#include "Algorithm.h"
#include "../Graph.h"
#include "PathTreeUtils/PathTree.h"

namespace path_tree {

    class PathTreeWrapper : public Algorithm {
    private:
        int alg_type = 1;
        PathTree *pt_ptr;
        PathTreeGraph g;
        int *sccmap;

    public:
        PathTreeWrapper(int alg_type = 1);

        std::string getName() const override;

        std::string getParams() const override;

        void reset() override;

        void construction(const Graph &graph) override;

        bool TC_haspath(int source, int target) override;

        long long getIndexSize() const override;
    };
}

#endif //XJUMP_PATHTREEWRAPPER_H

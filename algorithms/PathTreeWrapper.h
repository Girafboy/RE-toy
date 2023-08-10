#ifndef TC_PATHTREEWRAPPER_H
#define TC_PATHTREEWRAPPER_H


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
        explicit PathTreeWrapper(int alg_type = 1);

        std::string getName() const override;

        std::string getParams() const override;

        void reset() override;

        void construction(const Graph &graph) override;

        bool TC_haspath(int source, int target) override;

        unsigned long long getIndexSize() const override;
    };
}

#endif

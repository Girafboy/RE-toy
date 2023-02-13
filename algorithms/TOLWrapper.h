//
// Created by 王星力 on 2022/12/14.
//

#ifndef XJUMP_TOLWRAPPER_H
#define XJUMP_TOLWRAPPER_H

#include <string>

#include "Algorithm.h"
#include "../Graph.h"
#include "./TolUtils/utility1.h"

namespace tol {
    using namespace ::std;

//    const int VectorDefaultSize = 20;

    class TOLWrapper : public Algorithm {
    private:
        int style;
        int opr;
        iVector<int> labels;
        int** Index;
        int N = 0;

    public:
        TOLWrapper(int style = 0, int opr = 2);

        string getName() const override;

        string getParams() const override;

        void reset() override;

        void construction(const Graph &graph) override;

        bool TC_haspath(int source, int target) override;

        long long getIndexSize() const override;
    };

} // tol

#endif //XJUMP_TOLWRAPPER_H

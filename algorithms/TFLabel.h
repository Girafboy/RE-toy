#ifndef TC_TFLABEL_H
#define TC_TFLABEL_H

#include "Graph.h"
#include "Algorithm.h"

#include <vector>

namespace tf_label {

    using namespace ::std;

    class TFLabel : public Algorithm {
    private:
        int *dag, *topo;
        vector<int> *SL;

    public:
        TFLabel();

        std::string getName() const override;

        std::string getParams() const override;

        void reset() override;

        void construction(const Graph &graph) override;

        bool TC_haspath(int source, int target) override;

        unsigned long long getIndexSize() const override;
    };

} // tf_label

#endif //TC_TFLABEL_H

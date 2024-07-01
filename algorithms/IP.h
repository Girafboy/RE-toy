#ifndef TC_IP_H
#define TC_IP_H

#include "Graph.h"
#include "Algorithm.h"
#include "IpUtils/IPGraph.h"

namespace ip {

    class IP : public Algorithm {
    private:
        int k;
        int h;
        int mu;
        IPGraph g;

    public:
        explicit IP(int k = 2, int h = 2, int mu = 100);

        std::string getName() const override;

        std::string getParams() const override;

        void reset() override;

        void construction(const Graph &graph) override;

        bool TC_haspath(int source, int target) override;

        unsigned long long getIndexSize() const override;
    };

} // ip

#endif //TC_IP_H

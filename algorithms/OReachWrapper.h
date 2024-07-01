//
// Created by kang on 24-6-1.
//

#ifndef REACHABILITY_OREACHWRAPPER_H
#define REACHABILITY_OREACHWRAPPER_H

#include "Graph.h"
#include "Algorithm.h"
#include "lib/reachability/oreach.h"

#include <vector>
#include <string>

namespace o_reach {

    class OReachWrapper : public Algorithm {
    public:
        OReachWrapper();

        std::string getName() const override;

        std::string getParams() const override;

        void reset() override;

        void construction(const Graph &graph) override;

        void construction(const std::string &file_path);

        bool TC_haspath(int source, int target) override;

        unsigned long long getIndexSize() const override;

    private:
        graph_access input_graph;
        ReachabilityConfig config;
        std::unique_ptr<oreach> r;
    };

} // oreach

#endif //REACHABILITY_OREACHWRAPPER_H

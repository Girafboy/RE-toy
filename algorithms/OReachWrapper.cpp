//
// Created by kang on 24-6-1.
//

#include "OReachWrapper.h"

#include <iostream>
#include <iomanip>
#include <sys/stat.h>
#include "OreachUtils/extern/KaHIP/lib/tools/chronotimer.h"
#include "OreachUtils/extern/KaHIP/lib/io/graph_io.h"
#include "OreachUtils/extern/KaHIP/lib/data_structure/graph_access.h"
#include "OreachUtils/extern/KaHIP/lib/definitions.h"
#include "OreachUtils/app/parse_reachability_parameters.h"
#include "OreachUtils/app/reachabilityconfig.h"
#include "OreachUtils/lib/reachability/oreach.h"
#include "OreachUtils/lib/algorithms/bfs.h"
#include "OreachUtils/extern/KaHIP/lib/tools/random_functions.h"
#include "OreachUtils/extern/KaHIP/lib/algorithms/strongly_connected_components.h"
#include <algorithm>
#include "OreachUtils/lib/reachability/matrix/full_reach.h"

#include "OreachUtils/app/query_generator.h"

namespace o_reach {
    void OReachWrapper::construction(const Graph &graph) {
        if (graph_io::readDiGraph(input_graph, graph)) {
            throw std::runtime_error("Error reading Graph object.");
        }
        random_functions::setSeed(config.seed);
        r = std::make_unique<oreach>(input_graph, config);
        r->initialize();
        r->setFallbackMode(FB_PIBFS);
    }

    bool OReachWrapper::TC_haspath(int source, int target) {
        std::vector<Query> queries = {Query(source, target)};
        r->query<FB_PIBFS>(queries);
        return queries[0].isPositive();
    }

    std::string OReachWrapper::getName() const {
        return "O'Reach";
    }

    std::string OReachWrapper::getParams() const {
        return {};
    }

    void OReachWrapper::reset() {
        r.reset(nullptr);
    }

    unsigned long long OReachWrapper::getIndexSize() const {
        return r->getIndexSize();
    }

    void OReachWrapper::construction(const std::string &file_path) {
        if (graph_io::readDiGraph(input_graph, file_path)) {
            throw std::runtime_error("Error reading Graph object.");
        }
        random_functions::setSeed(config.seed);
        r = std::make_unique<oreach>(input_graph, config);
        r->initialize();
        r->setFallbackMode(FB_NONE);
    }

    OReachWrapper::OReachWrapper() = default;
} // oreach
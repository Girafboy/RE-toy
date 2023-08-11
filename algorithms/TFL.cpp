#include "TFL.h"

#include "TflUtils/pre_check.h"
#include "TflUtils/queryscc.h"

namespace tfl {
    TFL::TFL() = default;

    std::string TFL::getName() const {
        return "TFL";
    }

    std::string TFL::getParams() const {
        return std::string();
    }

    void TFL::construction(const Graph &graph) {
        tfl_pre_check::main_pre_check(graph);
        tfl_pre_check::after_pre_check();
        tfl_queryscc::before_queryscc(graph.size(), graph.getName());
        for (const string &s : {"_dag_label"s, "_TL"s, "_tlstart"s, "_topo_label"s}) {
            std::string to_delete_file_path = "../tmp/TFL/"s + graph.getName() + s;
            remove(to_delete_file_path.c_str());
        }
    }

    bool TFL::TC_haspath(int source, int target) {
        return tfl_queryscc::query_queryscc(source, target);
    }

    void TFL::reset() {
        tfl_queryscc::after_queryscc();
    }

    unsigned long long TFL::getIndexSize() const {
        size_t TL_size = 0;
        for (int i = 0; i < 2 * tfl_queryscc::tc_graph_size; ++i) {
            TL_size += tfl_queryscc::tlsize[i];
        }
        return (TL_size + 2 * tfl_queryscc::tc_graph_size) * sizeof(int);
    }
} // tfl
#include "DBLWrapper.h"

#include <vector>

namespace dbl {
    DBLWrapper::DBLWrapper() = default;

    std::string DBLWrapper::getName() const {
        return "DBL";
    }

    std::string DBLWrapper::getParams() const {
        return {};
    }

    void DBLWrapper::reset() {
        dbl = DBL();
    }

    void DBLWrapper::construction(const Graph &graph) {
        dbl.construct_index(graph);
    }

    bool DBLWrapper::TC_haspath(int source, int target) {
        std::vector<int> u = {source}, v = {target};
        int sz = 1;
        bool *result = new bool[sz];
        std::fill_n(result, sz, false);
        dbl.query(u, v, sz, result);
        bool ret = result[0];
        delete[] result;
        return ret;
    }

    unsigned long long DBLWrapper::getIndexSize() const {
        unsigned long long sz = 0ull;
        // V, E, csr_size
//        sz += 3 * sizeof(int);
//        for (const auto &s : dbl.adj) {
//            sz += s.size() * sizeof(int);
//        }
//        for (const auto &s : dbl.rev_adj) {
//            sz += s.size() * sizeof(int);
//        }
//        sz += dbl.ord.size() * sizeof(int);
//        sz += dbl.rev_ord.size() * sizeof(int);
        sz += dbl.dl_in.size() * (dl / 8);
        sz += dbl.dl_out.size() * (dl / 8);
        sz += dbl.bl_in.size() * (bl / 8);
        sz += dbl.bl_out.size() * (bl / 8);
//        sz += dbl.scc.size() * sizeof(int);
        sz += dbl.csr.size() * sizeof(int);
        sz += dbl.prefixsum.size() * sizeof(int);
//        sz += dbl.rev_csr.size() * sizeof(int);
//        sz += dbl.rev_prefixsum.size() * sizeof(int);
//        sz += dbl.dl_hash.size() * sizeof(int);
        return sz;
    }
} // dbl
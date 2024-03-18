#ifndef TC_PROFILE_H
#define TC_PROFILE_H

#include <string>
#include <vector>

struct Profile {
    std::string algorithm_name;
    std::string graph_name;
    std::string params;
    unsigned long long query_num;
    double average_has_path_time_ns;
    unsigned long long index_size;
};

#endif

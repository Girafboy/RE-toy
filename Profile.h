#ifndef TC_PROFILE_H
#define TC_PROFILE_H

#include <string>

struct Profile {
    std::string algorithm_name;
    std::string graph_name;
    std::string params;
    double preparation_time;
    double total_has_path_time;
    unsigned long long index_size;
};

#endif

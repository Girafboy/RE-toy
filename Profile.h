//
// Created by 王星力 on 2022/11/8.
//

#ifndef KANG_PROFILE_H
#define KANG_PROFILE_H

#include <string>

struct Profile {
    std::string algorithm_name;
    std::string graph_name;
    std::string params;
    double preparation_time;
    double total_has_path_time;
    long long index_size;
};

#endif //KANG_PROFILE_H

//
// Created by 王星力 on 2022/11/7.
//
#include "../Graph.h"

#ifndef KANG_ALGORITHM_H
#define KANG_ALGORITHM_H


class Algorithm {
public:
    virtual std::string getName() const = 0;
    virtual std::string getParams() const = 0;
    virtual void reset() = 0;
    virtual void construction(const Graph &graph) = 0;
    virtual bool TC_haspath(int source, int target) = 0;
    virtual ~Algorithm() = default;
    virtual long long getIndexSize() const = 0;
};


#endif //KANG_ALGORITHM_H

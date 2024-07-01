#include "Graph.h"

#ifndef TC_ALGORITHM_H
#define TC_ALGORITHM_H


class Algorithm {
public:
    virtual std::string getName() const = 0;

    virtual std::string getParams() const = 0;

    virtual void reset() = 0;

    virtual void construction(const Graph &graph) = 0;

    virtual bool TC_haspath(int source, int target) = 0;

    virtual ~Algorithm() = default;

    virtual unsigned long long getIndexSize() const = 0;
};


#endif

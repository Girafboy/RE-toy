//
// Created by 王星力 on 2022/12/27.
//

#ifndef XJUMP_UTIL_H
#define XJUMP_UTIL_H
// below are headers of C files;
#include<time.h>
// below are headers of C++ files
#include <bitset>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <map>
#include <set>
#include <stack>
#include <queue>
#include <deque>
#include <vector>
#include <list>
#include <iterator>
#include <algorithm>
#include <functional>
#include <utility>
#include <new>
#include <cstring>

namespace gripp {
    class item {
    public:
        unsigned node, pre, post, type;

        item(unsigned nd = 0, unsigned pr = 0,
             unsigned pt = 0, unsigned tp = 0) :
                node(nd), pre(pr), post(pt), type(tp) {}
    };

    class Util {
    public:
        static const unsigned MAX_NODE = 30000000U;
        static const unsigned MAX_EDGE = 200000000U;
        static const unsigned MAX_NAME_LEN = 1024;
        static const unsigned MAX_SHOWN_ITEM = 30;
        static const unsigned TREE = 0;
        static const unsigned NONTREE = 1;

        bool cmp(const item &i1, const item &i2) {
            return i1.pre < i2.pre;
        }
    };
}
#endif //XJUMP_UTIL_H

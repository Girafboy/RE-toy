//
// Created by 王星力 on 2022/11/9.
//

#ifndef KANG_BFL_H
#define KANG_BFL_H

#include <utility>
#include <vector>
#include "../Graph.h"
#include "Algorithm.h"

namespace bfl {

#ifndef K
#define K 5  // 1 2 3 5 10 20 50 100 200 500 1000
#endif
#ifndef D
#define D (320 * K)
#endif


    class BFL : public Algorithm {
    private:
        struct node {
            int N_O_SZ, N_I_SZ;
            int *N_O, *N_I;
            int vis;
            union {
                int L_in[K];
#if K > 8
                unsigned int h_in;
#else
                unsigned char h_in;
#endif
            };
            union {
                int L_out[K];
#if K > 8
                unsigned int h_out;
#else
                unsigned char h_out;
#endif
            };
            std::pair<int, int> L_interval;
        };

        std::vector<node> nodes;
        int vis_cur, cur;

        void read_graph(const Graph *graph_ptr);

        int h_in();

        int h_out();

        void dfs_in(node &u);

        void dfs_out(node &u);

        void index_construction();

        bool reach(const node &u, const node &v);

    public:
        explicit BFL();

        std::string getName() const override;

        std::string getParams() const override;

        void reset() override;

        void construction(const Graph &graph) override;

        bool TC_haspath(int source, int target) override;

        long long getIndexSize() const override;
    };

}
#endif //KANG_BFL_H

//
// Created by 王星力 on 2022/10/25.
//

#ifndef KANG_KANG_H
#define KANG_KANG_H

#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <functional>
#include <list>

#include "Algorithm.h"
#include "../Graph.h"

namespace kang {

    class Kang : public Algorithm {
    private:
        typedef std::vector<int> Jumps;
        struct Node {
            double volume_in = 0;
            double volume_out = 0;
            Jumps jump_in;
            Jumps jump_out;
            int degree_in = 0;
            int degree_out = 0;
            int color = 0;
            int level = 0;

            Node() {}
        };

        static std::vector<Node> nodes; // TODO: 原生数组
        static int color_cur;

        std::vector<std::list<int>> topo_adj_list;
        const Graph* graph;
        size_t x;

        void TC();

        void DFS(int u);

        void volume_update(std::vector<int>& main, std::vector<int>& minor, int level);

        void cutoff(std::vector<int>& main, std::vector<int>& minor, int level);

        Jumps* merge1(Jumps *jumps);

        Jumps* merge2(const Jumps *jumps_a, const Jumps *jumps_b);

        Jumps* mergek(const std::vector<const Jumps *> &jumps_list);

        static bool cover(const Jumps &sortedlist, const int target_order);

    public:
        Kang(int x);

        std::string getName() const override;

        std::string getParams() const override;

        void reset() override;

        void construction(const Graph &graph) override;

        bool TC_haspath(int source, int target) override;

        long long getIndexSize() const override;
    };
}

#endif //KANG_KANG_H

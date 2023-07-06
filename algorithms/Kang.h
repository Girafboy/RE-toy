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
            int preorder = 0;
            int postorder = 0;
            const Jumps *jump = nullptr;
            const Jumps *deep = nullptr;
            int level = 0;
            int visit = 0;

            Node() {}

            Node(int preorder, int postorder) : preorder(preorder), postorder(postorder) {}
        };

        static std::vector<Node> nodes; // TODO: 原生数组
        static int visit_cur;
        static int increno;

        std::vector<std::list<int>> topo_adj_list;
        size_t x;
        size_t MAX_LEVEL = 0;

        void TC();

        void DFS(int u);

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

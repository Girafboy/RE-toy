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
        struct Node {
            int preorder = 0;
            int postorder = 0;
            std::vector<int> *shallowjump = nullptr;
            std::vector<int> *deepjump = nullptr;
            std::vector<int> *fakejump = nullptr;
            int visit = 0;

            Node() {}

            Node(int preorder, int postorder) : preorder(preorder), postorder(postorder) {}
        };

        static std::vector<Node> nodes; // TODO: 原生数组

        std::vector<std::list<int>> topo_adj_list;
        size_t x;

        void TC();

        void DFS(int u);

        void merge(std::vector<std::vector<int> *> &jumps, std::vector<int> &merged_jump);

        void merge2(std::vector<int> *a, std::vector<int> *b, std::vector<int> &result);

        static bool cover(const std::vector<int> &sortedlist, const int target_order);

        void shrink(std::vector<int> &fakejump);

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

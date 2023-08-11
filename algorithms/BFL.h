#ifndef TC_BFL_H
#define TC_BFL_H

#include <utility>
#include <vector>
#include "../Graph.h"
#include "Algorithm.h"

namespace bfl {

    class BFL : public Algorithm {
    private:
        const int K;
        const int D;
        struct node {
            int K;
            int N_O_SZ = 0, N_I_SZ = 0;
            int *N_O = nullptr, *N_I = nullptr;
            int vis = 0;
            int *L_in = nullptr;
            void *h_in = nullptr;
            int *L_out = nullptr;
            void *h_out = nullptr;
            std::pair<int, int> L_interval;

            explicit node(int K) : K(K) {
                L_in = new int[K];
                L_out = new int[K];
                if (K > 8) {
                    h_in = new unsigned int(0);
                    h_out = new unsigned int(0);
                } else {
                    h_in = new unsigned char(0);
                    h_out = new unsigned char(0);
                }
            }

            node(const node &other) : K(other.K) {
                L_in = new int[K];
                L_out = new int[K];
                if (K > 8) {
                    h_in = new unsigned int(0);
                    h_out = new unsigned int(0);
                } else {
                    h_in = new unsigned char(0);
                    h_out = new unsigned char(0);
                }
            }

            ~node() {
                delete[] N_O;
                delete[] N_I;
                delete[] L_in;
                delete[] L_out;
                if (K > 8) {
                    delete static_cast<unsigned int *>(h_in);
                    delete static_cast<unsigned int *>(h_out);
                } else {
                    delete static_cast<unsigned char *>(h_in);
                    delete static_cast<unsigned char *>(h_out);
                }
            }
        };

        std::vector<node> nodes;
        int vis_cur = 0, cur = 0;

        void read_graph(const Graph *graph_ptr);

        int h_in();

        int h_out();

        void dfs_in(node &u);

        void dfs_out(node &u);

        void index_construction();

        bool reach(const node &u, const node &v);

    public:
        explicit BFL(int K);

        std::string getName() const override;

        std::string getParams() const override;

        void reset() override;

        void construction(const Graph &graph) override;

        bool TC_haspath(int source, int target) override;

        unsigned long long getIndexSize() const override;
    };

}
#endif

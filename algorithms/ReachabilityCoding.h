#ifndef REACHABILITY_CODING_H
#define REACHABILITY_CODING_H

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <functional>
#include <list>

#include "Algorithm.h"
#include "../Graph.h"


#define RANGE_MAX 0xFFFFFFFFUL
#define RANGE_HALF 0x80000000UL
#define RANGE_ONE_QUAR 0x40000000UL
#define RANGE_THREE_QUAR 0xC0000000UL
#define RANGE_MID_MIN 0x00000001UL

namespace rc {
    static int log2(int val) {
        int ret = 0;
        while (val > 1) {
            val >>= 1;
            ret++;
        }
        return ret;
    }

    class ReachabilityCoding : public Algorithm {
    private:
        struct Bits {
            unsigned char *data = nullptr;
            int size;
            int size_bytes;

            Bits() : data(nullptr), size(0), size_bytes(0) {}

            Bits(const Bits& other) : data(new unsigned char[other.size_bytes]), size(other.size), size_bytes(other.size_bytes) {
                std::memcpy(data, other.data, size_bytes);
            }

            ~Bits() {
                if (data)
                    delete[] data;
            }

            void init(int size) {
                this->size = size;
                this->size_bytes = (size+7)/8;
                this->data = new unsigned char[size_bytes];
                std::memset(data, 0, size_bytes);
            }

            void set(int pos) {
                data[pos >> 3] |= (0x80 >> (pos & 7));
            }

            bool get(int pos) const {
                return data[pos >> 3] & (0x80 >> (pos & 7));
            }

            void bits_or(const Bits &other) {
                for (int i = 0; i < other.size_bytes; i++) {
                    data[i] |= other.data[i];
                }
            }

            bool operator&(const Bits &other) const {
                for (int i = 0; i < size_bytes; i++) {
                    if (data[i] & other.data[i]) {
                        return true;
                    }
                }
                return false;
            }

            void append_zero() {
                size++;
                if ((size+7)/8 > size_bytes) {
                    unsigned char *tmp = data;
                    init(size);
                    std::memcpy(data, tmp, size_bytes-1);
                    delete []tmp;
                }
            }

            void append_one() {
                size++;
                if ((size+7)/8 > size_bytes) {
                    unsigned char *tmp = data;
                    init(size);
                    std::memcpy(data, tmp, size_bytes-1);
                    delete []tmp;
                }
                set(size-1);
            }

            void print() {
                for (int i = 0; i < size; i++) {
                    if (get(i)) {
                        std::cout << '1';
                    } else {
                        std::cout << '0';
                    }
                }
            }
        };

        struct Node {
            int topo_order;
            Bits *codes = nullptr;
            struct pair {
                int pos;
                float p0;

                pair(int pos, float p0) : pos(pos), p0(p0) {}
            };
            std::vector<pair> p0_pos;

            Node() {}

            ~Node() {
                if (codes)
                    delete[] codes;
            }
        };

        inline float get_p0(std::vector<Node::pair> &p0_pos, int pos, float p0_base) {
            for (auto &pair : p0_pos) {
                if (pair.pos >= pos) {
                    p0_base = pair.p0;
                } else {
                    break;
                }
            }
            return p0_base;
            // auto len = p0_pos.size();
            // const Node::pair *first = &p0_pos[0], *mid;
            // while (len) {
            //     mid = first + (len >> 1);
            //     if (pos > mid->pos)
            //         len = (len >> 1);
            //     else {
            //         first = mid + 1;
            //         len -= (len >> 1) + 1;
            //     }
            // }

            // return first->p0;
        }

        inline int get_chunk_num(int topo_order) const {
            return (topo_order + chunk_size - 1) / chunk_size;
        }

        std::vector<Node> nodes;
        float *connect_p0 = nullptr;
        int chunk_size;

        float encode(const Bits &bits, Bits &out, float p0, int cur, int len);
        void encode(Node &node);
        bool decode_check(const Bits &code, float p0, int cur, int len);
    public:
        ReachabilityCoding(int x);

        std::string getName() const override;

        std::string getParams() const override;

        void reset() override;

        void construction(const Graph &graph) override;

        bool TC_haspath(int source, int target) override;

        unsigned long long getIndexSize() const override;
    };
}

#endif //REACHABILITY_CODING_H

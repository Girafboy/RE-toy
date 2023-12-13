#ifndef ORSE_TOY_H
#define ORSE_TOY_H

#include <algorithm>
#include <cmath>
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


#define RANGE_MAX 0xFFFFFFFFU
#define RANGE_HALF 0x80000000U
#define RANGE_ONE_QUAR 0x40000000U
#define RANGE_THREE_QUAR 0xC0000000U


#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define bswap(x) __builtin_bswap32(x)
#else
#define bswap(x) x
#endif

typedef unsigned int fastfloat_t;

namespace orse_toy {
    class ORSE_Toy : public Algorithm {
    private:
        struct Bits {
            unsigned char *data = nullptr;
            int size;
            int size_bytes;

            Bits() : data(nullptr), size(0), size_bytes(0) {}

            Bits(const Bits &other) : data(new unsigned char[other.size_bytes]), size(other.size),
                                      size_bytes(other.size_bytes) {
                std::memcpy(data, other.data, size_bytes);
            }

            Bits& operator=(Bits &&other) {
                delete []data;
                data = other.data;
                size = other.size;
                size_bytes = other.size_bytes;
                other.data = nullptr;
                other.size = 0;
                other.size_bytes = 0;
            }

            ~Bits() {
                delete[] data;
            }

            void init(int size) {
                this->size = size;
                this->size_bytes = (size + 7) / 8;
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
                if ((size + 7) / 8 > size_bytes) {
                    unsigned char *tmp = data;
                    init(size);
                    std::memcpy(data, tmp, size_bytes - 1);
                    delete[] tmp;
                }
            }

            void append_one() {
                size++;
                if ((size + 7) / 8 > size_bytes) {
                    unsigned char *tmp = data;
                    init(size);
                    std::memcpy(data, tmp, size_bytes - 1);
                    delete[] tmp;
                }
                set(size - 1);
            }
        };

        struct Node {
            int topo_order;
            Bits *codes = nullptr;

            Node() = default;

            ~Node() {
                delete[] codes;
            }
        };

        struct pair {
            int pos;
            unsigned int p0;

            pair(int pos, unsigned int p0) : pos(pos), p0(p0) {}
        };

        static inline unsigned int get_p0(std::vector<pair> &p0_pos, int pos, unsigned int p0_default) {
            auto len = p0_pos.size();
            const pair *first = &p0_pos[0], *mid;
            while (len) {
                mid = first + (len >> 1);
                if (pos > mid->pos)
                    len = (len >> 1);
                else {
                    p0_default = mid->p0;
                    first = mid + 1;
                    len -= (len >> 1) + 1;
                }
            }

            return p0_default;
        }

        inline int get_chunk_num(int topo_order) const {
            return (topo_order + chunk_size - 1) / chunk_size;
        }

        static inline float approximation_ratio(unsigned int p0, unsigned int p0_base) {
            return ((32ULL << 32) + p0 * std::log2(1.0 / p0_base) +
                    ((1ULL << 32) - p0) * std::log2(1.0 / ((1ULL << 32) - p0_base))) /
                   ((32ULL << 32) + p0 * std::log2(1.0 / p0) +
                    ((1ULL << 32) - p0) * std::log2(1.0 / ((1ULL << 32) - p0)));
        }

        Node *nodes;
        unsigned int *connect_p0 = nullptr;
        std::vector<pair> *p0_pos;
        int chunk_size;
        float ratio;
        size_t n;

        unsigned int encode(Bits &bits, Bits &out, unsigned int p0, int cur, int len) const;

        void encode(Node &node);

        static bool decode_check(const Bits &code, fastfloat_t p0, fastfloat_t *p0_cur, int len);

    public:
        ORSE_Toy(int x, float r);

        std::string getName() const override;

        std::string getParams() const override;

        void reset() override;

        void construction(const Graph &graph) override;

        bool TC_haspath(int source, int target) override;

        unsigned long long getIndexSize() const override;
    };
}

#endif //ORSE_TOY_H

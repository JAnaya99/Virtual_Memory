#ifndef VIRTUAL_MEMORY_REPLACEMENT_ALGORITHMS_H
#define VIRTUAL_MEMORY_REPLACEMENT_ALGORITHMS_H

#include "Page.h"

//TODO: ADD COMMENTS.
namespace virtual_memory {
    namespace replacement_algorithms {
        class FIFO {
        public:
            bool operator()(const Page &a, const Page &b) {
                return a.GetArrivalNumber() < b.GetArrivalNumber();
            }
        };

        class LRU {
        public:
            bool operator()(const Page &a, const Page &b) {
                return a.GetTimestamp() < b.GetTimestamp();
            }
        };
    } // replacement_algorithms
} // virtual_memory

#endif //VIRTUAL_MEMORY_REPLACEMENT_ALGORITHMS_H
